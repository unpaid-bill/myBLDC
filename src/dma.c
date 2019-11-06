#include "dma.h"

void DMA_Init(DMA_TypeDef *dma){
    if (dma == DMA1)
    {
        RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    }
    
    DMA1_Channel1->CPAR = (int32_t)&(ADC1->DR);
    // DMA1_Channel1->CMAR = ;

}