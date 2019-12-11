#include "dma.h"

void DMA_Init(DMA_Channel_TypeDef *dma_channel, uint32_t cpar, uint32_t cmar){
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    myDelay(10);
    dma_channel->CPAR = cpar;
    dma_channel->CMAR = cmar;
    dma_channel->CNDTR = 6;
    dma_channel->CCR &= ~(DMA_CCR_PL_0 | DMA_CCR_PL_1);
    // memory icnrement
    // circular mode
    // set peripheral and memory sizes to 16b
    dma_channel->CCR |= DMA_CCR_MINC | DMA_CCR_CIRC 
                        | DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0
                        | DMA_CCR_TEIE | DMA_CCR_TCIE | DMA_CCR_HTIE;
    // dma_channel->CCR &= ~(DMA_CCR_HTIE);
    dma_channel->CCR |= DMA_CCR_EN;
    // enable
}