#ifndef __BLDC_H__
#define __BLDC_H__

#include <stdint.h>
#include "drv8323.h"

typedef struct
{
    uint16_t test_variable;
    drv8323_TypeDef drv;
    TIM_TypeDef tim;
    uint16_t ADC_samples[5];
    /* add FOC stuff here */
} motor_TypeDef;

void motor_init_drv(motor_TypeDef* m, SPI_TypeDef* spi);

#endif /* __BLDC_H__ */