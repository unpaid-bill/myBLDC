#include "uart.h"

#define UART_TIMEOUT 5000U

void UART_Init(USART_TypeDef *uart){
    // enable RCC for USART
    // RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    if(uart == USART1){
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    } else if (uart == USART2){
        RCC->APB2ENR |= RCC_APB1ENR_USART2EN;
    } else if (uart == USART3){
        RCC->APB2ENR |= RCC_APB1ENR_USART3EN;
    }
    // enable RCC for GPIO port of USART1 (GPIOA)
    // RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // enable RCC for alternate function clocks
    // RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // remap USART1 to alternate pins
    // AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;

    // setup GPIO pins for USART1
    // TX PB6
    // RX PB7
    // pin_mode(GPIOB, 6, GPIO_AF_PP);
    // pin_mode(GPIOB, 7, GPIO_IN_FL);

    // BAUDRATE = 115200;
    uart->BRR = 0b0000001000101100;
    uart->CR1 = 0b0010000000001100;
    uart->CR2 = 0x0000;
    uart->CR3 = 0x0000;
}

// TODO void UART_Baudrate(USART_TypeDef *uart, uint32_t baud){}

void UART_pin_Remap(USART_TypeDef *uart, uint8_t setting){
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
    // if (setting){    
    //     AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
    // } else {
    //     AFIO->MAPR &= (~AFIO_MAPR_USART1_REMAP);
    // }

    return;
}

void transmit_uart(USART_TypeDef *uart, char *ch, uint32_t len){
    int t = UART_TIMEOUT;
    char *p = ch;
    while(!(uart->SR & USART_SR_TXE)){
        t--;
        if (t<=0){
            break;
        }
    }
    uint32_t i = len;
    while(i > 0){
        i--;
        uart->DR = (*p++ & (uint8_t)0xFF);
        t = UART_TIMEOUT;
        while(!(uart->SR & USART_SR_TC)){
            t--;
            if (t<=0){
                break;
            }
        }
    }
}

void transmit_byte_uart(USART_TypeDef *uart, char ch){
    int t = UART_TIMEOUT;
    while(!(uart->SR & USART_SR_TXE)){
        t--;
        if (t<=0){
            break;
        }
    }
    uart->DR = (ch & (uint8_t)0xFF);
    t = UART_TIMEOUT;
    while(!(uart->SR & USART_SR_TC)){
        t--;
        if (t<=0){
            break;
        }
    }
}


void print_reg(uint32_t reg, uint8_t reg_sz){
    for (uint8_t i = 0; i < reg_sz; i++) {
        uint8_t shift = reg_sz - 1 - i;
        // if((i%4)==0) printf(" ");
        // if((i%8)==0) printf(" ");
        if((i%16)==0) printf(" ");
        if (reg & (1<<shift)) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
}
