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

motor_TypeDef m;

int main(void)
{   
    // SystemInit();
    // ITM_SendChar('A');
    SystemClock_Config();
    // RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    // AFIO->MAPR |= (AFIO_MAPR_SWJ_CFG_1);
    // DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;
    // pin_mode(GPIOB, 3, GPIO_AF_PP);
    // /* Configure Trace Port Interface Unit */
    // CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Enable access to registers
    // DWT->CTRL = 0x400003FE; // DWT needs to provide sync for ITM
    // ITM->LAR = 0xC5ACCE55; // Allow access to the Control Register
    // ITM->TPR = 0x0000000F; // Trace access privilege from user level code, please
    // ITM->TCR = 0x0001000D;
    // ITM->TER |= (1 << 0); // Only Enable stimulus port 0
    // ITM_SendChar('A');

    assess_reset_condition();

    watchdog_set_prescaler(32);
    watchdog_set_reload_reg(0xfff);
    watchdog_start();

    GPIO_Init();        printf("gpio initialized\n");
    UART_Init(USART3);  printf("uart initialized\n");
    SPI_Init(SPI1);     printf("SPI1 initialized\n");
    
    // printf("mem adress: ADC1->DR=%lu\n",(uint32_t)&(ADC1->DR));
    

    // NVIC_SetPriority(HardFault_IRQn, 0);
    // NVIC_EnableIRQ(HardFault_IRQn);
    // NVIC_SetPriority(BusFault_IRQn, 0);
    // NVIC_EnableIRQ(BusFault_IRQn);
    // NVIC_SetPriority(UsageFault_IRQn, 0);
    // NVIC_EnableIRQ(UsageFault_IRQn);


    /* ------------ DMA setup ------------ */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);
    DMA1_Channel1->CMAR = (uint32_t)ADC_samples;
    DMA1_Channel1->CNDTR = 5;
    DMA1_Channel1->CCR &= ~(DMA_CCR_PL_0 | DMA_CCR_PL_1);
    // DMA1_Channel1->CNDTR = (sizeof(ADC_samples)/sizeof(ADC_samples[0]));
    // memory icnrement
    // circular mode
    // set peripheral and memory sizes to 16b
    DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_CIRC 
                        | DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0
                        | DMA_CCR_TEIE | DMA_CCR_TCIE | DMA_CCR_HTIE;
    // DMA1_Channel1->CCR &= ~(DMA_CCR_HTIE);
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    // enable
    myDelay(500);
    printf("DMA1 Channel1 initialised\n");
    /* --------- DMA end of setup -------- */
    
    /* ------------ ADC setup ------------ */
    
    ADC1_Init();    printf("ADC1 initialized\n");

    ADC1->CR1 |= ADC_CR1_EOCIE;
    ADC1->CR2 |= ADC_CR2_CONT;
    ADC1->CR2 |= ADC_CR2_ADON;
    myDelay(10);
    ADC1->CR2 |= (ADC_CR2_RSTCAL);
    ADC1->CR2 |= (ADC_CR2_CAL);
    printf("adc on, calibrating...\n");
    while((ADC1->CR2) & (ADC_CR2_CAL)){
        // myDelay(1);
        ;
    }
    ADC1->CR2 |= ADC_CR2_ADON;


    myDelay(10);
    // print_reg(ADC1->CR2, 32);
    printf("adc on and calibrated\n");
    /* --------- ADC end of setup -------- */
    myDelay(10);
    myDelay(500);
    

    TIM1_Init();    printf("tim1 initialized\n");
    printf("CAL set LOW (off)\n");
    myDelay(10);
    // uint16_t tx;
    uint16_t rx;

    // drv8323_TypeDef drv;
    // drv8323_Init(&drv, SPI1); // assigns SPI1 peripheral to drv8323
    motor_init_drv(&m, SPI1);
    // drv_update(&drv);

    // sets OC_ADJ_SET to 24 (Vds = 1.043v)
    // Disable OCP
    // sets drv to 3-PWM mode
    // uint16_t set_reg = 0b01000000000 | 0b00000110000 | 0b00000001000;
    // uint16_t set_reg = (CR1_OC_ADJ_SET_24 | CR1_OCP_MODE_DISABLED | CR1_PWM_MODE_3);
    // uint16_t set_reg = (CR1_OC_ADJ_SET_7 | CR1_PWM_MODE_3);
    pin_set(ENABLE_Port, ENABLE_Pin);
    myDelay(100);
    // drv_write(&drv, DRV_CTRL_1, set_reg);
    printf("\nEnabled drv8323\n");

    transmit_uart(USART3, "test\n", 5);

    myDelay(50);

    printf("transmitting to drv\n");
    rx = drv_read_reg(&m.drv, DRV_GATE_DRIVE_HS);
    printf("Gate Drive HS register\t");   print_reg(rx, 16);
    if (rx == 0){
        error_handler();
    }
    printf("\n");
    
    TIM1_enable();


    uint32_t angle_a = 0;
    uint32_t angle_b = 120;
    uint32_t angle_c = 240;
    uint32_t max_duty_cycle = 7500;
    uint16_t duty_cycle_a;
    uint16_t duty_cycle_b;
    uint16_t duty_cycle_c;
    uint16_t angle_step = 30;
    
    watchdog_reload();
    printf("Interrupts enabled\n");
    
    // NVIC_SetPriority(DMA1_Channel1_IRQn, 1);
    // NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    // NVIC_SetPriority(ADC1_IRQn, 2);
    // NVIC_EnableIRQ(ADC1_IRQn);

    uint16_t t_del = 10;

    printf("while(1)\n");
    uint8_t fault = 0;
    while (1){
        if(fault){
            error_handler();
        }
        // myDelay(200);
        // printf("beep..\n");
        // fflush(stdout);
        // ASENSE, BSENSE, CSENSE, A_CURR, B_CURR
        // for(int i = 0; i < 4; i++){
        //     printf("%d, ", ADC_samples[i]);
        // }
        // printf("%d\n", ADC_samples[4]);
        
        duty_cycle_a = ((sin(angle_a * (PI/180))+1)/2)*max_duty_cycle;
        duty_cycle_b = ((sin(angle_b * (PI/180))+1)/2)*max_duty_cycle;
        duty_cycle_c = ((sin(angle_c * (PI/180))+1)/2)*max_duty_cycle;
        // global_angle = fmod(global_angle, 360);
        angle_a = (angle_a + angle_step) % 360;
        angle_b =  angle_a + 120;
        angle_c =  angle_a + 240;
        // printf("%d, %d, %d, %d\n", (uint32_t)duty_cycle_a, (uint32_t)duty_cycle_b, (uint32_t)duty_cycle_c, angle_a);

        TIM1->CCR1  = duty_cycle_a;
        TIM1->CCR2  = duty_cycle_b;
        TIM1->CCR3  = duty_cycle_c;

        watchdog_reload();
        myDelay(t_del);
    }



}


void error_handler(void){
    printf("error encountered!\n");
    while(1){
        pin_set(LED_FAULT_Port, LED_FAULT_Pin);
        myDelay(500);
        pin_reset(LED_FAULT_Port, LED_FAULT_Pin);
        myDelay(500);
        watchdog_reload();
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