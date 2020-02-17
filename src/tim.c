#include "tim.h"

/*
    TODO: add separate init functions for Init_PWM and other modes
*/
void TIM_Init(TIM_TypeDef* tim){
    if(tim == TIM1){
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // enable Tim1 clock
        // Tim1 clock is 64MHz

        tim->PSC   = 0;    // clock = 64MHz / (PSC+0) = 64/1 = 64.00MHz
        tim->ARR   = 1600; // frequency = frequency / ARR = 64000000/1600 = 40KHz

        tim->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // set output-compare 1 mode into PWM mode 1
        tim->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2; // set output-compare 2 mode into PWM mode 1
        tim->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2; // set output-compare 3 mode into PWM mode 1
        
        tim->CCR1  = 0; // reset all three channels to zero (off)
        tim->CCR2  = 0;
        tim->CCR3  = 0;
    } else if(tim == TIM2){
        printf("ATTEMPTING TO SETUP UN-IMPLEMENTED TIMER! (TIM2!)\n");
    } else if(tim == TIM3){
        printf("ATTEMPTING TO SETUP UN-IMPLEMENTED TIMER! (TIM3!)\n");
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
        tim->CCMR1 |= TIM_CCMR1_CC1S_1; // CC1S = '01'
        tim->CCMR2 |= TIM_CCMR1_CC2S_1; // CC2S = '01'
        tim->SMCR  |= TIM_SMCR_SMS_1 | TIM_SMCR_SMS_2; // SMS: 011 Encoder mode 3
        tim->ARR    = 2000; // max encoder count
    } else if(tim == TIM4){
        printf("SETUP TIMER! (TIM4!) - THIS TIMER IS UNTESTED\n");
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
        tim->PSC   = 4;     // clk frequency = 64MHz / (PSC+1) = 64/5 = 12.80MHz
        tim->ARR   = 65535; // period = 1/(frequency / ARR) = 1/(12.8MHz / 65535) = 5.12mS (~195Hz)
        tim->DIER |= TIM_DIER_UIE;
    } else {
        printf("ATTEMPTING TO SETUP TIMER THAT DOESN'T EXIST!\n");
        while(1){
            ;;
        }        
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