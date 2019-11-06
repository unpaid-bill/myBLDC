#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f103xb.h"

#define GPIO_OUT_PP  0x00000002
#define GPIO_OUT_OD  0x00000006
#define GPIO_AF_PP   0x0000000B
#define GPIO_AF_OD   0x0000000F
#define GPIO_IN_PULL 0x00000008
#define GPIO_IN_FL   0x00000004
#define GPIO_IN_AN   0x00000000

#define GPIO_PULLUP 0x01
#define GPIO_PULLDN 0x00

void pin_mode(GPIO_TypeDef  *port_reg, uint8_t pin, uint8_t mode);
void pin_speed(GPIO_TypeDef  *port_reg, uint8_t speed);
void pin_pullmode(GPIO_TypeDef  *port_reg, uint8_t pin, uint8_t pull);
void pin_set(GPIO_TypeDef  *port_reg, uint8_t pin);
void pin_reset(GPIO_TypeDef  *port_reg, uint8_t pin);
uint8_t pin_read(GPIO_TypeDef  *port_reg, uint8_t pin);

#endif // endif __GPIO_H__
