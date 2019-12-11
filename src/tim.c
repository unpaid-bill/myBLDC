#include "tim.h"

void TIM_Init(TIM_TypeDef* tim){
    if(tim == TIM1){
        RCC->APB2ENR |= (1<<11); // enable Tim1 clock

        uint16_t period = 30000;
        tim->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // set output-compare 1 mode into PWM mode 1
        tim->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2; // set output-compare 2 mode into PWM mode 1
        tim->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2; // set output-compare 3 mode into PWM mode 1
        
        tim->ARR   = period;
        tim->CCR1  = period/2;
        tim->CCR2  = period/4;
        tim->CCR3  = period/8;
    } else {
        printf("ATTEMPTING TO SETUP UN-IMPLEMENTED TIMER! (2-4?)\n");
    }
}

void TIM_enable(TIM_TypeDef* tim){
    tim->CR1   |= TIM_CR1_CEN;   //        
    tim->CCER  |= TIM_CCER_CC1E 
                 | TIM_CCER_CC2E 
                 | TIM_CCER_CC3E; // 
    tim->BDTR  |= TIM_BDTR_MOE;  // Main Output Enabled
}

void TIM1_PWM_Set_Freq(uint32_t freq){
    
}

void TIM1_cc1_mode(void){}