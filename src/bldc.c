#include "bldc.h"

static clarke_TypeDef clarke_tf(uint32_t a, uint32_t b); // returns alpha, beta
static park_TypeDef park_tf(uint32_t alpha, uint32_t beta, uint32_t angle); // returns d,q
static void reverse_park_tf();
static void reverse_clarke_tf();

void motor_init_drv(motor_TypeDef* m){
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

    uint16_t current_a = m->ADC_samples[0];
    uint16_t current_b = m->ADC_samples[1];
    // uint16_t current_c = m->ADC_samples[2];
    
    clarke_TypeDef clarke;
    park_TypeDef park;

    clarke = clarke_tf(current_a, current_b);
    park = park_tf(clarke.alpha, clarke.beta, m->angle);

    /*
        park PID control update
    */

   printf("clarke:%ld, %ld, %d\n", clarke.alpha, clarke.beta, m->angle);
   printf("Park:%ld, %ld\n", park.d, park.q);

    /*  calculate:
        
        duty_cycle_a
        duty_cycle_b
        duty_cycle_c
    */
    // duty_cycle_a = ((sin(angle_a * (PI/180))+1)/2)*max_duty_cycle;
    // duty_cycle_b = ((sin(angle_b * (PI/180))+1)/2)*max_duty_cycle;
    // duty_cycle_c = ((sin(angle_c * (PI/180))+1)/2)*max_duty_cycle;
    // global_angle = fmod(global_angle, 360);
    // angle_a = (angle_a + angle_step) % 360;
    // angle_b =  angle_a + 120;
    // angle_c =  angle_a + 240;
    // printf("%d, %d, %d, %d\n", (uint32_t)duty_cycle_a, (uint32_t)duty_cycle_b, (uint32_t)duty_cycle_c, angle_a);
}

clarke_TypeDef clarke_tf(uint32_t a, uint32_t b){
    clarke_TypeDef result;
    // returns alpha, beta

    result.alpha = a;
    result.beta = ((1/sqrt(3)) * (2*b + a));

    return result;
}

park_TypeDef park_tf(uint32_t alpha, uint32_t beta, uint32_t angle){ 
    park_TypeDef result;
    float theta;
    // returns d,q
    
    theta = angle* (PI / 180);
    result.d = ( cos(theta) * alpha) + (sin(theta) * beta); // DATATYPES??!!
    result.q = (-sin(theta) * alpha) + (cos(theta) * beta);
    return result;
}

void motor_enable(motor_TypeDef *m){
    pin_set(m->enable_port, m->enable_pin);
    // change this function to _actually_ enable the *motor* and not just turn on the chip?
}