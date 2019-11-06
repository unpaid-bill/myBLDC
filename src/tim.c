#include "tim.h"

void TIM1_Init(void){
    RCC->APB2ENR |= (1<<11); // enable Tim1 clock

    uint16_t period = 30000;
    TIM1->CCMR1 |= (0b110 << 4);
    TIM1->CCMR1 |= (0b110 << 12);
    TIM1->CCMR2 |= (0b110 << 4);
    
    TIM1->ARR   = period;
    TIM1->CCR1  = period/2;
    TIM1->CCR2  = period/4;
    TIM1->CCR3  = period/8;
}

void TIM1_enable(void){
    TIM1->CR1   |= TIM_CR1_CEN;   //        
    TIM1->CCER  |= TIM_CCER_CC1E; // 
    TIM1->CCER  |= TIM_CCER_CC2E; // 
    TIM1->CCER  |= TIM_CCER_CC3E; // 
    TIM1->BDTR  |= TIM_BDTR_MOE;  // Main Output Enabled
}

void TIM1_PWM_Set_Freq(uint32_t freq){
    
}

void TIM1_cc1_mode(void){}