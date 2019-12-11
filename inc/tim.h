#ifndef __TIM_H__
#define __TIM_H__

#include "stm32f103xb.h"
#include <stdio.h>

void TIM_Init(TIM_TypeDef* tim);
void TIM_enable(TIM_TypeDef* tim);

#endif // endif __GPIO_H__