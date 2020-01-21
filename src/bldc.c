#include "bldc.h"

static clarke_TypeDef clarke_tf(float a, float b); // returns alpha, beta
static park_TypeDef park_tf(uint32_t alpha, uint32_t beta, uint32_t angle); // returns d,q
static clarke_TypeDef inverse_park_tf(park_TypeDef park, uint32_t theta);
static phase_TypeDef inverse_clarke_tf(clarke_TypeDef clarke);

/* move these somewhere? */
    // printf("Interrupts enabled\n");

    // NVIC_SetPriority(DMA1_Channel1_IRQn, 1);
    // NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    // NVIC_SetPriority(ADC1_IRQn, 2);
    // NVIC_EnableIRQ(ADC1_IRQn);

/*
    initialises motor peripherals, configures gate driver and PWM
*/
void bldc_init_drv(motor_TypeDef* m){
    printf("initialising motor...\n");
    m->fault = 0;
    SPI_Init(m->spi);   printf("SPI1 initialized\n");
    TIM_Init(m->tim);   printf("tim1 initialized\n");
    /* ------------ DMA setup ------------ */
    uint32_t cpar = (uint32_t)&(ADC1->DR);
    uint32_t cmar = (uint32_t)&m->ADC_voltage_A;
    DMA_Init(DMA1_Channel1, cpar, cmar);
    myDelay(10);
    printf("DMA1 Channel1 initialised\n");
    /* --------- DMA end of setup -------- */
    
    /* ------------ ADC setup ------------ */
    ADC_Init(ADC1);    printf("ADC1 initialized\n");
    /* --------- ADC end of setup -------- */

    /* set PWM to OFF!! */
    bldc_PWM_OFF(m);
    drv8323_Init(&m->drv, m->spi);
    pin_set(m->enable_port, m->enable_pin);
    // motor enabled!
    drv8323_update(&m->drv);
    drv8323_enable_all_fault_reporting(&m->drv);
    if (m->drv.gate_drive_HS != 0b0000001111111111){
        printf("drv comms failed\n");
        m->fault = 1;
        return;
    }
    // sets drv to 3-PWM mode
    drv8323_3pwm_mode(&m->drv);
    /* 
        sets overcurrent VDS_LVL to ?? (Vds = ??) default is 0.75v. Leave there for now
    */ 
    m->initialised = 1;
    printf("initialised motor\n");
}

/*
    Turns the motor ON (as in, ready and spinning)
*/
void bldc_enable(motor_TypeDef *m){
    if((m->initialised) && (m->fault == 0)){
        printf("enabling motor...\n");
    } else {
        printf("Error: attempting to enable motor before initialising!\n");
    }

    TIM_enable(TIM1); // turn on timers

    pin_set(m->enable_port, m->enable_pin);
    // change this function to _actually_ enable the *motor* and not just turn on the chip?
}

void bldc_update(motor_TypeDef* m){
    drv8323_update(&m->drv);
    /* 
        read faults
    */

    /*  update timer with speed */
    if(m->foc){
        bldc_update_foc_params(m);
    } else {
        bldc_update_openloop(m);
    }

    bldc_update_PWM(m);

    /*
        x
    */

}

void bldc_PWM_OFF(motor_TypeDef *m){
    m->duty_cycle_a = 0;
    m->duty_cycle_b = 0;
    m->duty_cycle_c = 0;

    bldc_update_PWM(m);
}

void bldc_update_PWM(motor_TypeDef *m){
        m->tim->CCR1 = m->duty_cycle_a;
        m->tim->CCR2 = m->duty_cycle_b;
        m->tim->CCR3 = m->duty_cycle_c;
}

void bldc_update_openloop(motor_TypeDef* m){
    if (m->openloop_stage > 5){
        m->openloop_stage = 0;
    }

    switch (m->openloop_stage)
    {
    case 0:
        m->duty_cycle_a = m->target_speed;
        m->duty_cycle_b = 0;
        m->duty_cycle_c = 0;
        break;
    case 1:
        m->duty_cycle_a = m->target_speed;
        m->duty_cycle_b = m->target_speed;
        m->duty_cycle_c = 0;
        break;
    case 2:
        m->duty_cycle_a = 0;
        m->duty_cycle_b = m->target_speed;
        m->duty_cycle_c = 0;
        break;
    case 3:
        m->duty_cycle_a = 0;
        m->duty_cycle_b = m->target_speed;
        m->duty_cycle_c = m->target_speed;
        break;
    case 4:
        m->duty_cycle_a = 0;
        m->duty_cycle_b = 0;
        m->duty_cycle_c = m->target_speed;
        break;
    case 5:
        m->duty_cycle_a = m->target_speed;
        m->duty_cycle_b = 0;
        m->duty_cycle_c = m->target_speed;
        break;
    
    default:
        break;
    }

    m->openloop_stage++;
}

void bldc_update_foc_params(motor_TypeDef* m){

    float current_a = bldc_get_phase_current(m->ADC_current_A);
    float current_b = bldc_get_phase_current(m->ADC_current_B);
    // float current_c = bldc_get_phase_current(m->ADC_current_C);
    
    clarke_TypeDef  clarke;
    park_TypeDef    park;

    clarke = clarke_tf(current_a, current_b);
    park = park_tf(clarke.alpha, clarke.beta, m->angle);

    /*
        park PID control update
    */

    printf("clarke:%f, %f, %d\n", clarke.alpha, clarke.beta, m->angle);
    printf("Park:%f, %f\n", park.d, park.q);

    /*  calculate duty cycles   */
    park_TypeDef     i_park = park; // the one from the PID loop
    clarke_TypeDef   i_clarke; 
    phase_TypeDef    phase;
 
    i_clarke = inverse_park_tf(i_park, m->angle);
    phase = inverse_clarke_tf(i_clarke);

    m->duty_cycle_a = phase.a;
    m->duty_cycle_b = phase.b;
    m->duty_cycle_c = phase.c;
 
    printf("new park: d:%f, q:%f\n", i_park.d, i_park.q);
    printf("new clarke: alpha:%f, beta:%f\n", i_clarke.alpha, i_clarke.beta);
    printf("phase: A:%f, B:%f, C:%f\n", phase.a, phase.b, phase.c);
    // duty_cycle_a = ((sin(angle_a * (PI/180))+1)/2)*max_duty_cycle;
    // duty_cycle_b = ((sin(angle_b * (PI/180))+1)/2)*max_duty_cycle;
    // duty_cycle_c = ((sin(angle_c * (PI/180))+1)/2)*max_duty_cycle;
    // global_angle = fmod(global_angle, 360);
    // angle_a = (angle_a + angle_step) % 360;
    // angle_b =  angle_a + 120;
    // angle_c =  angle_a + 240;
    // printf("%d, %d, %d, %d\n", (uint32_t)duty_cycle_a, (uint32_t)duty_cycle_b, (uint32_t)duty_cycle_c, angle_a);
}

/*
Get real voltage from ADC sample
*/
float bldc_get_phase_voltage(uint16_t v_adc_in){
    return ((float)v_adc_in/4096) * VDD_voltage / PHASE_DIVIDER_RATIO;
}

float bldc_get_phase_current(uint16_t i_adc_in){
    return ((float)i_adc_in/4096) * VDD_voltage / CURRENT_GAIN;
}

void bldc_set_target_speed(motor_TypeDef* m, uint16_t speed){
    m->target_speed = speed;
}

/* do some BEMF sensing and calculate angle (approx)
    TODO: some zero crossing stuff
*/
void bldc_angle_observer(motor_TypeDef *m){
    /* use the following:... */
    /*m->real_voltage_A;
    m->real_voltage_B;
    m->real_voltage_C;*/

    /* and calculate: */
    m->angle;
}

/*
    clarke transform
*/
clarke_TypeDef clarke_tf(float a, float b){
    clarke_TypeDef result;
    // returns alpha, beta

    result.alpha = a;
    result.beta = (ONE_OVER_SQRT_THREE * (2*b + a));

    return result;
}

/*
    park transform
*/
park_TypeDef park_tf(uint32_t alpha, uint32_t beta, uint32_t angle){ 
    // #warning DATATYPES!!!!
    park_TypeDef result;
    float theta;
    // returns d,q
    
    theta = angle* (PI / 180);
    result.d = ( cos(theta) * alpha) + (sin(theta) * beta); // DATATYPES??!!
    result.q = (-sin(theta) * alpha) + (cos(theta) * beta);
    return result;
}

/* 
inverse clarke transform
takes in alpha and beta, returns a,b,c phase values
*/
phase_TypeDef inverse_clarke_tf(clarke_TypeDef clarke){

    // #warning DATATYPES!!!!
    phase_TypeDef phase;

    phase.a = clarke.alpha;
    phase.b = (-clarke.alpha + SQRT_THREE*clarke.beta)/2;
    phase.c = (-clarke.alpha - SQRT_THREE*clarke.beta)/2;

    return phase;
}

/*
inverse park transform
takes in d,q, angle, returns alpha & beta
*/
clarke_TypeDef inverse_park_tf(park_TypeDef park, uint32_t theta){

    // #warning DATATYPES!!!!
    clarke_TypeDef clarke;

    clarke.alpha = park.d*cos(theta) - park.q*sin(theta);
    clarke.alpha = park.d*sin(theta) + park.q*cos(theta);

    return clarke;
}

/*
measures inductance of the overall motor
*/
// https://www.nxp.com/docs/en/application-note/AN4680.pdf
void bldc_measure_inductance(void){
    // needs to be spinning to run this measurement
    // L =  V / (dI/dt)
    //   = (V * dt) / dI

    // measure current over a period of time to get the change (dI)
    // count how long that change takes (dt)
    // V is the input bus voltage
    // I assume the inductance is that of the whole motor in that measurement period
    ;
}
// https://www.nxp.com/docs/en/application-note/AN4680.pdf
void bldc_measure_resistance(void){
    // locks the current to specific value, by way of q_set

    // R = V / I
    
    // measure input bus voltage
    // measure current, both averaged

}

/*
calculates motor's inductance and resistance
*/
void bldc_calibrate(void){
    // https://www.nxp.com/docs/en/application-note/AN4680.pdf
    bldc_measure_inductance();
    bldc_measure_resistance();

}