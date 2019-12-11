#ifndef __DMA_H__
#define __DMA_H__

#include "stm32f103xb.h"
#include "hardware.h"

void DMA_Init(DMA_Channel_TypeDef *dma_channel, uint32_t cpar, uint32_t cmar);

#endif // endif __DMA_H__