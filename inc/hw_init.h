#ifndef __HW_INIT_H__
#define __HW_INIT_H__

#include "hardware.h"
#include "stm32f103xb.h"
#include "gpio.h"
#include "dma.h"

void GPIO_Init(void);
void SystemClock_Config(void);
void assess_reset_condition(void);

#endif /* __HW_INIT_H__ */