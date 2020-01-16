#include "tim.h"

void TIM_Init(TIM_TypeDef* tim){
    if(tim == TIM1){
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // enable Tim1 clock

        uint16_t period = 30000;
        tim->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // set output-compare 1 mode into PWM mode 1
        tim->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2; // set output-compare 2 mode into PWM mode 1
        tim->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2; // set output-compare 3 mode into PWM mode 1
        
        tim->ARR   = period;
        tim->CCR1  = 0;
        tim->CCR2  = 0;
        tim->CCR3  = 0;
    } else if(tim == TIM2){
        printf("ATTEMPTING TO SETUP UN-IMPLEMENTED TIMER! (TIM2!)\n");
    } else if(tim == TIM3){
        printf("ATTEMPTING TO SETUP UN-IMPLEMENTED TIMER! (TIM3!)\n");
    } else if(tim == TIM4){
        printf("SETUP TIMER! (TIM4!) - THIS TIMER IS UNTESTED\n");
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
        tim->PSC = 4; // frequency = 64MHz / (PSC+1) = 64/5 = 12.80MHs
        tim->ARR = 65535; // period = 1/(frequency / ARR) = 1/(12.8MHz / 35535) = 5.12mS
    } else {
        printf("ATTEMPTING TO SETUP TIMER THAT DOESN'T EXIST!\n");        
    }
}

void TIM_enable(TIM_TypeDef* tim){
    tim->CR1   |= TIM_CR1_CEN;   // enable timer
    if(tim == TIM1){
        tim->CCER  |= TIM_CCER_CC1E 
                    | TIM_CCER_CC2E 
                    | TIM_CCER_CC3E; // 
        tim->BDTR  |= TIM_BDTR_MOE;  // Main Output Enabled
    }
}

void TIM_disable(TIM_TypeDef* tim){
    tim->CR1  &= ~TIM_CR1_CEN;  //
    if (tim == TIM1){
        tim->BDTR &= ~TIM_BDTR_MOE; // switch off Main Output Enable
    }
}

void TIM1_PWM_Set_Freq(uint32_t freq){
    
}

void TIM1_cc1_mode(void){}