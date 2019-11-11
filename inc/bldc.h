#ifndef __BLDC_H__
#define __BLDC_H__

#include <stdint.h>
#include "drv8323.h"
#include "math.h"

typedef struct
{
    uint16_t angle;
    drv8323_TypeDef drv;
    TIM_TypeDef* tim;
    uint16_t ADC_samples[6];
    uint16_t target_speed;
    uint32_t duty_cycle_a;
    uint32_t duty_cycle_b;
    uint32_t duty_cycle_c;
    uint8_t  fault;
    /* add FOC stuff here */
} motor_TypeDef;

typedef struct
{
    uint32_t d;
    uint32_t q;
} park_TypeDef;

typedef struct
{
    uint32_t alpha;
    uint32_t beta;
} clarke_TypeDef;

void motor_init_drv(motor_TypeDef* m, SPI_TypeDef* spi, TIM_TypeDef* tim);
void motor_update(motor_TypeDef* m);
void update_foc_params(motor_TypeDef* m);
clarke_TypeDef clarke_tf(uint32_t a, uint32_t b); // returns alpha, beta
park_TypeDef park_tf(uint32_t alpha, uint32_t beta, uint32_t angle); // returns d,q
void reverse_park_tf();
void reverse_clarke_tf();

#endif /* __BLDC_H__ */