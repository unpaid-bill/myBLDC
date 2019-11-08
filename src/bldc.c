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

    // alpha = a
    // beta = ((1/m.sqrt(3)) * (2*b + a))

    return result;
}

park_TypeDef park_tf(uint32_t alpha, uint32_t beta, uint32_t angle){ 
    park_TypeDef result;
    // returns d,q
    
    // theta = m.radians(angle)
    // d = ( m.cos(theta) * alpha) + (m.sin(theta) * beta)
    // q = (-m.sin(theta) * alpha) + (m.cos(theta) * beta)
    return result;
}