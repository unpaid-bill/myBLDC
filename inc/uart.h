#ifndef __UART_H__
#define __UART_H__

#include "stm32f103xb.h"
#include <stdio.h>
#include "gpio.h"

void UART_Init(USART_TypeDef *uart);
void UART_pin_Remap(USART_TypeDef *uart, uint8_t setting);

void transmit_uart(USART_TypeDef *uart, char *ch, uint32_t len);
void transmit_byte_uart(USART_TypeDef *uart, char ch);

void print_reg(uint32_t reg, uint8_t reg_sz);

#endif // endif __UART_H__
