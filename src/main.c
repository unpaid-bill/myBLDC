#include "hardware.h"
#include <stdio.h>
#include <math.h>
#include "stm32f1xx.h"
#include "spi.h"
#include "drv8323.h"
#include "uart.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "dma.h"
#include "iwdg.h"
#include "hw_init.h"
#include "irq.h"
#include "bldc.h"

/* Private function prototypes -----------------------------------------------*/

void SysTick_Handler(void)
{
    myTicks++;
}

void myDelay(uint32_t mS){
    myTicks = 0;
    while(myTicks<mS);
}

void enable_SWO(void){
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= (AFIO_MAPR_SWJ_CFG_1);
    DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;
    pin_mode(GPIOB, 3, GPIO_AF_PP);
    /* Configure Trace Port Interface Unit */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Enable access to registers
    DWT->CTRL = 0x400003FE; // DWT needs to provide sync for ITM
    ITM->LAR = 0xC5ACCE55; // Allow access to the Control Register
    ITM->TPR = 0x0000000F; // Trace access privilege from user level code, please
    ITM->TCR = 0x0001000D;
    ITM->TER |= (1 << 0); // Only Enable stimulus port 0
    ITM_SendChar('A');
}

motor_TypeDef m;
uint8_t update_flag = 0;

int main(void)
{   
    // SystemInit();
    // ITM_SendChar('A');
    SystemClock_Config();
    // enable_SWO();

    assess_reset_condition(); // print any reset cause

    watchdog_set_prescaler(32);
    watchdog_set_reload_reg(0xfff);
    watchdog_start();

    GPIO_Init();        printf("gpio initialized\n");
    UART_Init(USART3);  printf("uart initialized\n");
    // SPI_Init(SPI1);     printf("SPI1 initialized\n");
    
    // printf("mem adress: ADC1->DR=%lu\n",(uint32_t)&(ADC1->DR));
    

    // NVIC_SetPriority(HardFault_IRQn, 0);
    // NVIC_EnableIRQ(HardFault_IRQn);
    // NVIC_SetPriority(BusFault_IRQn, 0);
    // NVIC_EnableIRQ(BusFault_IRQn);
    // NVIC_SetPriority(UsageFault_IRQn, 0);
    // NVIC_EnableIRQ(UsageFault_IRQn);

    printf("ADC pointer: %ld\n",(uint32_t)&(ADC1->DR));
    printf("struct pointer: %ld\n",(uint32_t)&(m.ADC_voltage_A));
    /* ------------ DMA setup ------------ */
    // RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    // DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);
    // DMA1_Channel1->CMAR = (uint32_t)&(m.ADC_voltage_A);
    // DMA1_Channel1->CNDTR = 6;
    // DMA1_Channel1->CCR &= ~(DMA_CCR_PL_0 | DMA_CCR_PL_1);
    // // memory icnrement
    // // circular mode
    // // set peripheral and memory sizes to 16b
    // DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_CIRC 
    //                     | DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0
    //                     | DMA_CCR_TEIE | DMA_CCR_TCIE | DMA_CCR_HTIE;
    // // DMA1_Channel1->CCR &= ~(DMA_CCR_HTIE);
    // DMA1_Channel1->CCR |= DMA_CCR_EN;
    // // enable
    // myDelay(500);
    // printf("DMA1 Channel1 initialised\n");
    /* --------- DMA end of setup -------- */
    
    /* ------------ ADC setup ------------ */
    
    // ADC_Init(ADC1);    printf("ADC1 initialized\n");

    // ADC1->CR1 |= ADC_CR1_EOCIE;
    // ADC1->CR2 |= ADC_CR2_CONT;
    // ADC1->CR2 |= ADC_CR2_ADON;
    // myDelay(10);
    // ADC1->CR2 |= (ADC_CR2_RSTCAL);
    // ADC1->CR2 |= (ADC_CR2_CAL);
    // printf("adc on, calibrating...\n");
    // while((ADC1->CR2) & (ADC_CR2_CAL)){
    //     // myDelay(1);
    //     ;
    // }
    // ADC1->CR2 |= ADC_CR2_ADON;
    // myDelay(10);
    // // print_reg(ADC1->CR2, 32);
    // printf("adc on and calibrated\n");
    /* --------- ADC end of setup -------- */
    myDelay(10);

    printf("CAL set LOW (off)\n");
    myDelay(10);
    // uint16_t tx;
    uint16_t rx;

    m.tim = TIM1;
    m.spi = SPI1;
    m.enable_pin  = ENABLE_Pin;
    m.enable_port = ENABLE_Port;
    m.cal_pin  = CAL_Pin;
    m.cal_port = CAL_Port;
    m.nfault_pin  = nFAULT_Pin;
    m.nfault_port = nFAULT_Port;
    m.initialised = 0;
    motor_init_drv(&m);

    printf("reading ADC voltages:\n");
    printf("m.ADC_voltage_A: %f\n", bldc_get_phase_voltage(&m, 0));
    printf("m.ADC_voltage_B: %f\n", bldc_get_phase_voltage(&m, 1));
    printf("m.ADC_voltage_C: %f\n", bldc_get_phase_voltage(&m, 2));
    printf("m.ADC_current_A: %f\n", m.real_current_A);
    printf("m.ADC_current_B: %f\n", m.real_current_B);
    printf("m.ADC_current_C: %f\n", m.real_current_C);
    /* Confirmed! ADC should be able to write to struct address space*/
    // float test = 55.4631;
    // printf("float test: %f\n", test); //prints: float test: 55.463100

    transmit_uart(USART3, "test\n", 5);

    // myDelay(50);

    printf("transmitting to drv\n");
    rx = drv8323_read_reg(&m.drv, DRV_GATE_DRIVE_HS);
    printf("Gate Drive HS register\t");   print_reg(rx, 16);
    if (rx == 0){
        error_handler(45);
    }
    printf("successful response\n");
    
    TIM_enable(TIM1);
    // move into bldc.c!!
    
    watchdog_reload();
    // printf("Interrupts enabled\n");
    
    // NVIC_SetPriority(DMA1_Channel1_IRQn, 1);
    // NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    // NVIC_SetPriority(ADC1_IRQn, 2);
    // NVIC_EnableIRQ(ADC1_IRQn);

    uint16_t t_del = 3000;

    printf("while(1)\n");
    m.fault = 0;

    /* ----------------------------- MAIN LOOP START -------------------------------- */
    while (1){
        myDelay(t_del);
        
        printf("m.ADC_voltage_A: %d\n", m.ADC_voltage_A);
        printf("m.ADC_voltage_B: %d\n", m.ADC_voltage_B);
        printf("m.ADC_voltage_C: %d\n", m.ADC_voltage_C);
        printf("m.ADC_current_A: %d\n", m.ADC_current_A);
        printf("m.ADC_current_B: %d\n", m.ADC_current_B);
        printf("m.ADC_current_C: %d\n", m.ADC_current_C);

        if(m.fault){
            error_handler(m.fault);
        }

        if(update_flag){
            printf("beep..\n");
            
            update_foc_params(&m);
            motor_update(&m);

            watchdog_reload();
        }
    }
    /* ------------------------------ MAIN LOOP END -------------------------------- */
}

void error_handler(uint8_t f){
    printf("error code %d encountered!\n", f);
    while(1){
        pin_set(LED_FAULT_Port, LED_FAULT_Pin);
        myDelay(500);
        pin_reset(LED_FAULT_Port, LED_FAULT_Pin);
        myDelay(500);

        if(f!=45){
            watchdog_reload();
        }
    }
}

void blink_fault(void){
    pin_reset(LED_FAULT_Port, LED_FAULT_Pin);
    myDelay(50);
    pin_set(LED_FAULT_Port, LED_FAULT_Pin);
    myDelay(50);
    pin_reset(LED_FAULT_Port, LED_FAULT_Pin);
    myDelay(50);
    pin_set(LED_FAULT_Port, LED_FAULT_Pin);
}