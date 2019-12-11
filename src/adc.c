#include "adc.h"


void ADC_Init(ADC_TypeDef* adc){
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // pin_mode(GPIOA, 1, GPIO_IN_AN);
    // pin_mode(GPIOA, 2, GPIO_IN_AN);
    // pin_mode(GPIOA, 3, GPIO_IN_AN);

    adc->SMPR2 |= (uint32_t)(0b111 << ADC_SMPR2_SMP1_Pos);
    adc->SMPR2 |= (uint32_t)(0b111 << ADC_SMPR2_SMP2_Pos);
    adc->SMPR2 |= (uint32_t)(0b111 << ADC_SMPR2_SMP3_Pos);
    adc->SMPR2 |= (uint32_t)(0b111 << ADC_SMPR2_SMP8_Pos);
    adc->SMPR2 |= (uint32_t)(0b111 << ADC_SMPR2_SMP9_Pos);

    adc->SQR1 |= ADC_SQR2_SQ7_2;
    adc->SQR3 |= (ADC_SQR3_SQ1_0)\
                | (ADC_SQR3_SQ2_0 | ADC_SQR3_SQ2_1)\
                | (ADC_SQR3_SQ3_0 | ADC_SQR3_SQ3_1 | ADC_SQR3_SQ3_2)\
                | (ADC_SQR3_SQ4_3)\
                | (ADC_SQR3_SQ5_0 | ADC_SQR3_SQ5_3);

    adc->CR1 |= ADC_CR1_SCAN;
    adc->CR2 |= ADC_CR2_DMA;

    adc->CR1 |= ADC_CR1_EOCIE;
    adc->CR2 |= ADC_CR2_CONT;
    adc->CR2 |= ADC_CR2_ADON;
    myDelay(10);
    adc->CR2 |= (ADC_CR2_RSTCAL);
    adc->CR2 |= (ADC_CR2_CAL);
    printf("adc on, calibrating...\n");
    while((adc->CR2) & (ADC_CR2_CAL)){
        // myDelay(1);
        ;
    }
    adc->CR2 |= ADC_CR2_ADON;
    myDelay(10);
    // print_reg(ADC1->CR2, 32);
    printf("adc on and calibrated\n");
}