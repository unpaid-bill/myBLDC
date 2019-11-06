#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f103xb.h"

void delay_ms(uint32_t milliseconds);
void delay_ns(uint32_t nanoseconds);
void delay_s(uint32_t seconds);

#endif // endif __DELAY_H__