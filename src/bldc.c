#include "bldc.h"

static clarke_TypeDef clarke_tf(float a, float b); // returns alpha, beta
static park_TypeDef   park_tf(uint32_t alpha, uint32_t beta, uint32_t angle); // returns d,q
static clarke_TypeDef inverse_park_tf(park_TypeDef park, uint32_t theta);
static phase_TypeDef  inverse_clarke_tf(clarke_TypeDef clarke);

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
    m->max_duty_cycle = m->tim->ARR;
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

    /* --------- encoder setup --------- */
    if(m->use_encoder){
        // make Encoder an object?

        // setup TIM3
        TIM_Init(TIM3);  printf("TIM3 initialised\n");
        // setup DMA to put into angle variable
        cpar = 0;
        cmar = 0;
        DMA_Init(DMA1_Channel2, cpar, cmar); // Check this function!!!!
        // setup external interrupt on zero pin
            // view 10.2.4 in refermence manual for details
        printf("initialised encoder???\n");
    }
    /* ----- end of encoder setup ------ */

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
    printf("drv8323 initialised!\n");
    /* 
        sets overcurrent VDS_LVL to ?? (Vds = ??) default is 0.75v. Leave there for now
    */ 
    
    pid_set_tunings(&m->q_pid, 100, 100, 100);
    pid_set_tunings(&m->d_pid, 100, 100, 100);
    m->q_set = 0;
    m->d_set = 0;
    printf("PID params intitialised (NEED TO TUNE PID!!!\n");

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
    bldc_PWM_OFF(m);
}

void bldc_update(motor_TypeDef* m){
    m->now_t = SysTick->VAL;
    drv8323_update(&m->drv);
    /* 
        read faults
    */

   /* update angle position */
    bldc_angle_update(m);

    /* Back EMF measurement */
    // bldc_get_phase_voltage(&m->ADC_voltage_A);
    // bldc_get_phase_voltage(&m->ADC_voltage_b);
    // bldc_get_phase_voltage(&m->ADC_voltage_C);


    /*  update timer with speed */
    if(m->use_foc){
        bldc_update_foc_params(m);
    } else {
        bldc_update_openloop(m);
    }

    bldc_update_PWM(m);

    m->lastrun_t = m->now_t;

}

void bldc_PWM_OFF(motor_TypeDef *m){
    m->duty_cycle_a = 0;
    m->duty_cycle_b = 0;
    m->duty_cycle_c = 0;

    bldc_update_PWM(m);
}

static void restrict_duty_cycle(motor_TypeDef *m){
    uint32_t max = m->max_duty_cycle;
    if(m->duty_cycle_a > max){
        m->duty_cycle_a = max;
    }
    if(m->duty_cycle_b > max){
        m->duty_cycle_b = max;
    }
    if(m->duty_cycle_c > max){
        m->duty_cycle_c = max;
    }
}

void bldc_update_PWM(motor_TypeDef *m){
        restrict_duty_cycle(m);
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
    // ticks since last run
    uint32_t delta_t = m->now_t - m->lastrun_t;

    float current_a = bldc_get_phase_current(m->ADC_current_A);
    float current_b = bldc_get_phase_current(m->ADC_current_B);
    // float current_c = bldc_get_phase_current(m->ADC_current_C);
    
    clarke_TypeDef  clarke;
    park_TypeDef    park;

    /* 3-phase to two phase */
    clarke = clarke_tf(current_a, current_b);

    /* stationary frame to rotating frame */
    park = park_tf(clarke.alpha, clarke.beta, m->angle);

    /* park PID control update */
    park_TypeDef i_park; // the one from the PID loop
    pid_compute(&m->q_pid, delta_t, m->q_set, park.q);
    pid_compute(&m->d_pid, delta_t, m->d_set, park.d);
    i_park.q = m->q_pid.output;
    i_park.d = m->d_pid.output;

    printf("clarke:%f, %f, %d\n", clarke.alpha, clarke.beta, m->angle);
    printf("Park:%f, %f\n", park.d, park.q);

    /* calculate duty cycles */
    clarke_TypeDef   i_clarke; 
    phase_TypeDef    phase;
 
    i_clarke = inverse_park_tf(i_park, m->angle);
    if (m->use_SVPWM){
        phase = bldc_svpwm(i_clarke);
    } else {
        phase = inverse_clarke_tf(i_clarke);

        m->duty_cycle_a = phase.a;
        m->duty_cycle_b = phase.b;
        m->duty_cycle_c = phase.c;
    
        printf("new park: d:%f, q:%f\n", i_park.d, i_park.q);
        printf("new clarke: alpha:%f, beta:%f\n", i_clarke.alpha, i_clarke.beta);
        printf("phase: A:%f, B:%f, C:%f\n", phase.a, phase.b, phase.c);
    }
}

void bldc_update_sine_mode(motor_TypeDef* m){
    /*
        take angle, get equiv sine value
        give each duty cycle equivalent values

        a_angle = angle
        b_angle = (angle + 120) % 360
        c_angle = (angle + 240) % 360

        m->duty_cycle_a = sin_table[a_angle]
        m->duty_cycle_b = sin_table[b_angle]
        m->duty_cycle_c = sin_table[C_angle]
    */
}

/*
Space Vector Pulse Width Modulation (svpwm)
*/
phase_TypeDef bldc_svpwm(clarke_TypeDef input){
    phase_TypeDef phase;

    uint8_t segment = 1;
    switch (segment)
    {
    case 0:
        phase.a = 0;
        phase.b = 0;
        phase.c = 0;
        break;
    default:
        phase.a = 0;
        phase.b = 0;
        phase.c = 0;
        break;
    }

    return phase;
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

void bldc_set_target_torque(motor_TypeDef *m, uint16_t q, uint16_t d){
    printf("setting speed to %d\n", q);
    m->q_set = q;
    m->d_set = d;
}


void bldc_angle_update(motor_TypeDef *m){
    if(m->use_encoder){
        // encoder code
        // update m->angle
        // convert encoder count to angle (degrees)
    } else {
        bldc_angle_observer(m);
    }
}

/* do some BEMF sensing and calculate angle (approx)
    TODO: some zero crossing stuff
    
    See http://cas.ensmp.fr/~praly/Telechargement/Journaux/2010-IEEE_TPEL-Lee-Hong-Nam-Ortega-Praly-Astolfi.pdf
*/
void bldc_angle_observer(motor_TypeDef *m){
    /* use the following:... */
    /*m->real_voltage_A;
    m->real_voltage_B;
    m->real_voltage_C;*/

    /* and calculate: */
    m->angle = 0;
}

void bldc_velocity_observer(motor_TypeDef *m){
    /* 
        takes either the time between full rotations (if going fast enough) 
        OR
        change in angle per unit time
        to get a hopefully accurate velocity value
    */

    /*
        run this every ~0.5-1.0 second (unless doing velocity control????)
        use the RTC interrupt to trigger this timing.
    */
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
void bldc_measure_inductance(motor_TypeDef *m){
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
void bldc_measure_resistance(motor_TypeDef *m){
    m->q_set = 100; //100mA
    /*
    while (not_timedout && not_complete)
    {
        // code
    }
    */
    // locks the current to specific value, by way of q_set

    // R = V / I
    
    // measure input bus voltage
    // measure current, both averaged

}

/*
calculates motor's inductance and resistance
*/
void bldc_calibrate(motor_TypeDef *m){
    // https://www.nxp.com/docs/en/application-note/AN4680.pdf
    bldc_measure_inductance(m);
    bldc_measure_resistance(m);

}