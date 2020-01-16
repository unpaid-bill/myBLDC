#ifndef __HARDWARE_H__
#define __HARDWARE_H__

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
// #include "gpio.h"
// #include "spi.h"

#ifndef STM32F103xB
    #define STM32F103xB
#endif

/* Private define ------------------------------------------------------------*/
#define PI 3.14159265358979323846264


#define nFAULT_Pin      9     // B
#define nFAULT_Port     GPIOB
#define CAL_Pin         8     // B 
#define CAL_Port        GPIOB
#define ENABLE_Pin      7     // B
#define ENABLE_Port     GPIOB
/* PWM Pins */
#define INH_A_Pin       10    // A
#define INH_B_Pin       9     // A
#define INH_C_Pin       8     // A
#define INH_A_Port      GPIOA
#define INH_B_Port      GPIOA
#define INH_C_Port      GPIOA
#define INL_A_Pin       15     // B
#define INL_B_Pin       14     // B
#define INL_C_Pin       13     // B
#define INL_A_Port      GPIOB
#define INL_B_Port      GPIOB
#define INL_C_Port      GPIOB
/* LEDs Pin */
#define LED_R_Pin       6     // B
#define LED_G_Pin       5     // B
#define LED_B_Pin       4     // B
#define LED_R_Port      GPIOB
#define LED_G_Port      GPIOB
#define LED_B_Port      GPIOB
/* voltage sense Pins */
#define ASENSE_Pin      1      // A
#define BSENSE_Pin      2      // A
#define CSENSE_Pin      3      // A
#define ASENSE_Port     GPIOA
#define BSENSE_Port     GPIOA
#define CSENSE_Port     GPIOA
/* current sense Pins */
#define SOA_Pin         0      // B
#define SOB_Pin         1      // B
#define SOC_Pin         0      // A
#define SOA_Port        GPIOB
#define SOB_Port        GPIOB
#define SOC_Port        GPIOA
/* SPI Pins */
#define SPI_PORT        GPIOA
#define SPI_Pin_NSS     4
#define SPI_Pin_SCK     5
#define SPI_Pin_MISO    6
#define SPI_Pin_MOSI    7
/* USART1 Pins */
#define USART_PORT     GPIOB
#define USART_Pin_TX    10
#define USART_Pin_RX    11

// GPIO_PortPin_TypeDef nFAULT_gpio = (GPIO_PortPin_TypeDef){.port=nFAULT_Port, .pin=nFAULT_Pin};


/* Variable decleration ------------------------------------------------------*/
volatile uint32_t myTicks;


/* Function prototypes -------------------------------------------------------*/
void myDelay(uint32_t mS);
void blink_fault(void);
void error_handler(uint8_t f);


#endif /* __HARDWARE_H__ */