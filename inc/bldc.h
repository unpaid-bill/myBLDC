#ifndef __BLDC_H__
#define __BLDC_H__

#include <stdint.h>
#include "drv8323.h"
#include "spi.h"
#include "tim.h"
#include "dma.h"
#include "adc.h"
#include "math.h"

#define ONE_OVER_SQRT_THREE 0.5773502692f
#define SQRT_THREE          1.732050808f

#define PHASE_DIVIDER_RATIO 0.1282051282f // 68K over 10K. 10/(10+68)
#define CURRENT_GAIN        10.0f
static float VDD_voltage = 3.30;

typedef struct
{
    uint16_t angle;
    drv8323_TypeDef drv;
    SPI_TypeDef* spi;
    TIM_TypeDef* tim;
    GPIO_TypeDef* enable_port;
    GPIO_TypeDef* cal_port;
    GPIO_TypeDef* nfault_port;
    uint8_t  enable_pin;
    uint8_t  cal_pin;
    uint8_t  nfault_pin;
    uint16_t ADC_voltage_A;
    uint16_t ADC_voltage_B;
    uint16_t ADC_voltage_C;
    uint16_t ADC_current_A;
    uint16_t ADC_current_B;
    uint16_t ADC_current_C;
    uint16_t target_speed;
    uint32_t duty_cycle_a;
    uint32_t duty_cycle_b;
    uint32_t duty_cycle_c;
    uint8_t  fault;
    uint8_t  initialised;
    uint8_t  foc;
    uint8_t  openloop_stage;
    /* add FOC stuff here */
} motor_TypeDef;

typedef struct
{
    float d;
    float q;
} park_TypeDef;

typedef struct
{
    float alpha;
    float beta;
} clarke_TypeDef;

typedef struct{
    float a;
    float b;
    float c;
} phase_TypeDef;

void  bldc_init_drv(motor_TypeDef* m);
void  bldc_update(motor_TypeDef* m);
void  bldc_update_openloop(motor_TypeDef* m);
void  bldc_update_foc_params(motor_TypeDef* m);
void  bldc_update_sine_mode(motor_TypeDef* m);
void  bldc_enable(motor_TypeDef *m);
float bldc_get_phase_voltage(uint16_t adc_in);
float bldc_get_phase_current(uint16_t adc_in);
void bldc_set_target_speed(motor_TypeDef* m, uint16_t speed);
void  bldc_PWM_OFF(motor_TypeDef *m);
void  bldc_update_PWM(motor_TypeDef *m);
void  bldc_calibrate(void);

#endif /* __BLDC_H__ */