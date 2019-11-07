#include "irq.h"



void DMA1_Channel1_IRQHandler(void){
    // pin_set(GPIOB, 11);
    if(DMA1->ISR & DMA_ISR_TEIF1){
        printf("DMA error\n");
        DMA1->IFCR |= (DMA_IFCR_CGIF1 | DMA_IFCR_CTEIF1);
    } else if(DMA1->ISR & DMA_ISR_HTIF1){
        // printf("DMA half transfer\n");
        DMA1->IFCR |= (DMA_IFCR_CGIF1 | DMA_IFCR_CHTIF1);
    } else if(DMA1->ISR & DMA_ISR_TCIF1){
        // printf("DMA transfer complete\n");
        DMA1->IFCR |= (DMA_IFCR_CGIF1 | DMA_IFCR_CTCIF1);
    }else if(DMA1->ISR & DMA_ISR_GIF1){
        // printf("DMA global interrupt\n");
        DMA1->IFCR |= DMA_IFCR_CGIF1;
    }
}

void ADC1_2_IRQHandler(void){
    // pin_set(GPIOB, 11);
    blink_fault();
    printf("ADC interrupt\n");
}


// void HardFault_Handler(void){
//     pin_set(LED_FAULT_Port, LED_FAULT_Pin);
// }
// void BusFault_Handler(void){
//     pin_set(LED_FAULT_Port, LED_FAULT_Pin);
// }
// void UsageFault_Handler(void){
//     pin_set(LED_FAULT_Port, LED_FAULT_Pin);
// }
