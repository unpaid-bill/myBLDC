#ifndef __SPI_H__
#define __SPI_H__

// #include "stm32f1xx_hal.h"
#include "stm32f103xb.h"
#include "hardware.h"
#include "gpio.h"

#define SPI_TIMEOUT 1000
#define SPI_CS_PIN 4
#define SPI_CS_PORT GPIOA

void SPI_Init(SPI_TypeDef *spi);
void SPI_Transfer(SPI_TypeDef *spi, uint16_t data);
uint16_t SPI_Transcieve(SPI_TypeDef *spi, uint16_t TxData);


#endif // endif __SPI_H__
