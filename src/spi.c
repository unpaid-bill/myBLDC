#include "spi.h"

void SPI_Init(SPI_TypeDef *spi){

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN;
    // PA4, NSS,  cnf = AFPP, mode = 50MHz
    // PA5, SCK,  cnf = AFPP, mode = 50MHz
    // PA6, MISO, cnf = input floating, mode = input
    // PA7, MOSI, cnf = AFPP, mode = 50MHz

    // spi->CR1 =   0b0000100100111101;
    // BIDIMODE    0   2-line unidirectional mode
    // BIDIOE      x   not one line bidirectional
    // CRCEN       0   CRC disabled
    // CRCNEXT     0   /X. No CRC
    // RXONLY      0   transmit and receive
    // SSM         0   software slave management
    // LSBFIRST    0   MSB first
    // SPE         0   SPI NOT enabled (enabled below)
    // CPOL        0   low when idle
    spi->CR1 |= SPI_CR1_DFF                 // 16 bit mode
              | SPI_CR1_SSI                 // slave select high
              | SPI_CR1_BR_0 | SPI_CR1_BR_1 // fpclk/16    arbitrary setting
              | SPI_CR1_MSTR                // Master configuration
              | SPI_CR1_CPHA;               // second edge

    spi->CR2      = 0b0000000000000100;
    // TXEIE       0   Tx buffer interupt not enabled
    // RXEIE       0   Rx buffer interupt not enabled
    // ERRIE       0   error interupt not enabled
    // SSOE        1   SS output is enabled
    // TXDMAEN     0   Tx buffer DMA disabled
    // RXDMAEN     0   Rx buffer DMA disabled
    // CR2 = xxxxxxxx000xx100

    spi->I2SCFGR  = 0x0000;
    // I2SMOD      0   SPI mode selected
    // I2SCFGR = xxxx0xxxxxxxxxxx

    // enable SPI1
    spi->CR1  |= SPI_CR1_SPE;
}

void SPI_Transfer(SPI_TypeDef *spi, uint16_t data){
    // Wait until SPI is not busy anymore
    while (spi->SR & (SPI_SR_BSY)){}

    pin_reset(SPI_PORT, SPI_Pin_NSS);
    // Write data to be transmitted to the SPI data register
	spi->DR = data;

	while (!(spi->SR & (SPI_SR_TXE))){}
    while (spi->SR & (SPI_SR_BSY)){}
    pin_set(SPI_PORT, SPI_Pin_NSS);
    spi->DR;
}

// uint16_t SPI1_Receive(void){}

uint16_t SPI_Transcieve(SPI_TypeDef *spi, uint16_t TxData){
    uint16_t RxData = 0;
    // Wait until SPI is not busy anymore
    while (spi->SR & (SPI_SR_BSY)){}

    pin_reset(SPI_PORT, SPI_Pin_NSS);
    // Write data to be transmitted to the SPI data register
	spi->DR = TxData;

    uint16_t timeout = 1000;
    while(!(spi->SR & SPI_SR_TXE)){
        if(timeout == 0){
            break;
            printf("SPI transmit timeout\n");
        }
        timeout--;
    }
    timeout = 1000;
    while(!(spi->SR & SPI_SR_RXNE)){
        if(timeout == 0){
            break;
            printf("SPI receive timeout\n");
        }
        timeout--;
    }
    
    RxData = spi->DR;

    timeout = 1000;
    while(!(spi->SR & SPI_SR_TXE)){
        if(timeout == 0){
            break;
            printf("SPI TXE (cleanup) timeout\n");
        }
        timeout--;
    }
    timeout = 1000;
    while(!(spi->SR & SPI_SR_BSY)){
        if(timeout == 0){
            break;
            printf("SPI busy timeout\n");
        }
        timeout--;
    }
    pin_set(SPI_PORT, SPI_Pin_NSS);
    return RxData;
}
