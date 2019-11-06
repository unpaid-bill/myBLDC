#include "adc.h"


void ADC1_Init(void){
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // pin_mode(GPIOA, 1, GPIO_IN_AN);
    // pin_mode(GPIOA, 2, GPIO_IN_AN);
    // pin_mode(GPIOA, 3, GPIO_IN_AN);

    ADC1->SMPR2 |= (uint32_t)(0b111 << ADC_SMPR2_SMP1_Pos);
    ADC1->SMPR2 |= (uint32_t)(0b111 << ADC_SMPR2_SMP2_Pos);
    ADC1->SMPR2 |= (uint32_t)(0b111 << ADC_SMPR2_SMP3_Pos);
    ADC1->SMPR2 |= (uint32_t)(0b111 << ADC_SMPR2_SMP8_Pos);
    ADC1->SMPR2 |= (uint32_t)(0b111 << ADC_SMPR2_SMP9_Pos);

    ADC1->SQR1 |= ADC_SQR2_SQ7_2;
    ADC1->SQR3 |= (ADC_SQR3_SQ1_0)\
                | (ADC_SQR3_SQ2_0 | ADC_SQR3_SQ2_1)\
                | (ADC_SQR3_SQ3_0 | ADC_SQR3_SQ3_1 | ADC_SQR3_SQ3_2)\
                | (ADC_SQR3_SQ4_3)\
                | (ADC_SQR3_SQ5_0 | ADC_SQR3_SQ5_3);

    ADC1->CR1 |= ADC_CR1_SCAN;
    ADC1->CR2 |= ADC_CR2_DMA;

}