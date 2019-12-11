#include "bldc.h"

static clarke_TypeDef clarke_tf(float a, float b); // returns alpha, beta
static park_TypeDef park_tf(uint32_t alpha, uint32_t beta, uint32_t angle); // returns d,q
static clarke_TypeDef inverse_park_tf(park_TypeDef park, uint32_t theta);
static phase_TypeDef inverse_clarke_tf(clarke_TypeDef clarke);

void motor_init_drv(motor_TypeDef* m){
    printf("initialising motor...\n");
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

    drv8323_Init(&m->drv, m->spi);
    pin_set(m->enable_port, m->enable_pin);
    // motor enabled! ensure things aren't spinning yet?!
    drv8323_update(&m->drv);
    drv8323_enable_all_fault_reporting(&m->drv);
    // sets drv to 3-PWM mode
    drv8323_3pwm_mode(&m->drv);
    /* 
        sets OC_ADJ_SET to 24 (Vds = 1.043v)
    */ 
    m->initialised = 1;
    printf("initialised motor\n");
}

void motor_enable(motor_TypeDef *m){
    if((m->initialised) && (m->fault == 0)){
        printf("enabling motor...\n");
    } else {
        printf("Error: attempting to enable motor before initialising!\n");
    }
    pin_set(m->enable_port, m->enable_pin);
    // change this function to _actually_ enable the *motor* and not just turn on the chip?
}

void motor_update(motor_TypeDef* m){
    drv8323_update(&m->drv);
    /* 
        read faults
    */
    /*
        update timer with speed
    */
        m->tim->CCR1 = m->duty_cycle_a;
        m->tim->CCR2 = m->duty_cycle_b;
        m->tim->CCR3 = m->duty_cycle_c;
    /*
        x
    */

}

void update_foc_params(motor_TypeDef* m){

    float current_a = m->real_current_A;
    float current_b = m->real_current_B;
    // uint16_t current_c = m->ADC_samples[2];
    
    clarke_TypeDef  clarke;
    park_TypeDef    park;

    clarke = clarke_tf(current_a, current_b);
    park = park_tf(clarke.alpha, clarke.beta, m->angle);

    /*
        park PID control update
    */

    printf("clarke:%f, %f, %d\n", clarke.alpha, clarke.beta, m->angle);
    printf("Park:%f, %f\n", park.d, park.q);

    /*  calculate:
        
        duty_cycle_a
        duty_cycle_b
        duty_cycle_c
    */
    park_TypeDef     i_park; // the one from the PID loop
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

float bldc_get_phase_voltage(motor_TypeDef *m, uint8_t index){
    float duty_cycle = -1;
    if (index > 2){
        return duty_cycle;
    }
    switch (index)
    {
    case 0:
        duty_cycle = (float)m->duty_cycle_a;
        break;
    case 1:
        duty_cycle = (float)m->duty_cycle_a;
        break;
    case 2:
        duty_cycle = (float)m->duty_cycle_a;
        break;
    default:
        break;
    }

    return duty_cycle;
}

clarke_TypeDef clarke_tf(float a, float b){
    clarke_TypeDef result;
    // returns alpha, beta

    result.alpha = a;
    result.beta = ((1/sqrt(3)) * (2*b + a));

    return result;
}

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

phase_TypeDef inverse_clarke_tf(clarke_TypeDef clarke){
    // takes in alpha and beta, returns a,b,c phase values

    // #warning DATATYPES!!!!
    phase_TypeDef phase;

    phase.a = clarke.alpha;
    phase.b = (-clarke.alpha + sqrt(3)*clarke.beta)/2;
    phase.c = (-clarke.alpha - sqrt(3)*clarke.beta)/2;

    return phase;
}

clarke_TypeDef inverse_park_tf(park_TypeDef park, uint32_t theta){
    // takes in d,q, angle, returns alpha & beta

    // #warning DATATYPES!!!!
    clarke_TypeDef clarke;

    clarke.alpha = park.d*cos(theta) - park.q*sin(theta);
    clarke.alpha = park.d*sin(theta) + park.q*cos(theta);

    return clarke;
}