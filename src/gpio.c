#include "gpio.h"

void pin_mode(GPIO_TypeDef  *port_reg, uint8_t pin, uint8_t mode){
    volatile uint32_t *configreg;
    uint32_t config;
    uint8_t  pin_offset;
    configreg = (pin < 8) ? &port_reg->CRL : &port_reg->CRH;
    pin_offset = (pin < 8) ? pin : pin - 8;
    config = 0xFFFFFFFF ^ ((0b1111) << (pin_offset * 4));
    *configreg &= config;
    *configreg |= mode << (pin_offset * 4);
}

void pin_speed(GPIO_TypeDef  *port_reg, uint8_t speed){

}

void pin_pullmode(GPIO_TypeDef  *port_reg, uint8_t pin, uint8_t pull){
    uint32_t config;
    config = 0xFFFFFFFF & (1 << pin);
    config &= (pull << pin);
    port_reg->ODR |= config;    
}

void pin_set(GPIO_TypeDef  *port_reg, uint8_t pin){
    port_reg->ODR |= (1<<pin);
}

void pin_reset(GPIO_TypeDef  *port_reg, uint8_t pin){
    port_reg->BRR |= (1<<pin);
}

uint8_t pin_read(GPIO_TypeDef  *port_reg, uint8_t pin){
    uint8_t boolean = (port_reg->IDR & (1<<pin)) ? 1 : 0;
    return boolean;
}