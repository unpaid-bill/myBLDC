#include "bldc.h"

void motor_init_drv(motor_TypeDef* m, SPI_TypeDef* spi, TIM_TypeDef* tim){
    m->drv.spi = spi;
    m->tim = tim;
    drv8323_Init(&m->drv, m->drv.spi);
}

void motor_update(motor_TypeDef* m){
    drv_update(&m->drv);
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