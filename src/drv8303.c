#include "drv8303.h"
#include "spi.h"
#include "main.h"
#include "uart.h"
#include "gpio.h"


void drv8303_Init(drv8303_TypeDef *drv, SPI_TypeDef *spi){
    drv->spi = spi;
}

uint16_t drv_transceive(drv8303_TypeDef *drv, uint16_t tx_reg){
    //tx_reg = 0b1000100000000000;

    SPI_Transfer(drv->spi, tx_reg);
    // printf("transmitting\t");  print_reg(tx_reg, 16);
    uint16_t rx_reg = SPI_Transcieve(drv->spi, 0b1000000000000000);

    return rx_reg;
}

uint16_t drv_write(drv8303_TypeDef *drv, uint32_t drv_reg, uint16_t payload){
    //tx_reg = 0b1000100000000000;
    payload &= 0b0000011111111111;
    uint16_t tx_reg = 0;
    tx_reg |= (drv_reg & 0b0111100000000000);
    tx_reg |= payload;
    SPI_Transfer(drv->spi, tx_reg);
    uint16_t rx_reg = SPI_Transcieve(drv->spi, 0b1000000000000000);

    return rx_reg;
}

uint16_t drv_read(drv8303_TypeDef *drv, uint32_t drv_reg){
    //tx_reg = 0b1000100000000000;
    uint16_t tx_reg = 0;
    tx_reg |= drv_reg;
    tx_reg |= (1<<15);
    // tx_reg |= payload;

    // printf("reading\t");  print_reg(tx_reg, 16);

    SPI_Transfer(drv->spi, tx_reg);
    uint16_t rx_reg = SPI_Transcieve(drv->spi, 0b1000000000000000);

    return rx_reg;
}


void drv_write_reg(drv8303_TypeDef *drv, uint16_t reg){

}

void drv_read_reg(drv8303_TypeDef *drv, uint16_t reg){
    
}

void drv_update(drv8303_TypeDef *drv){

    SPI_Transfer(drv->spi, 0b1000000000000000);
    drv->SR1 = SPI_Transcieve(drv->spi, 0b1001000000000000);
    drv->SR2 = SPI_Transcieve(drv->spi, 0b1010000000000000);
    drv->CR1 = SPI_Transcieve(drv->spi, 0b1011000000000000);
    drv->CR2 = SPI_Transcieve(drv->spi, 0b1000000000000000);
    
}