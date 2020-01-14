#include "hardware.h"
#include <stdio.h>
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
    SystemClock_Config();
    // enable_SWO();

    assess_reset_condition(); // print any reset cause

    GPIO_Init();        printf("gpio initialized\n");
    UART_Init(USART3);  printf("uart initialized\n");        
    
    pin_set(LED_R_Port,   LED_R_Pin);
    myDelay(100);
    pin_reset(LED_R_Port,   LED_R_Pin);
    myDelay(100);
    pin_set(LED_G_Port,   LED_G_Pin);
    myDelay(100);
    pin_reset(LED_G_Port,   LED_G_Pin);
    myDelay(100);
    pin_set(LED_B_Port,   LED_B_Pin);
    myDelay(100);
    pin_reset(LED_B_Port,   LED_B_Pin);
    myDelay(100);

    watchdog_set_prescaler(32);
    watchdog_set_reload_reg(0xfff);
    watchdog_start();
    // NVIC_SetPriority(HardFault_IRQn, 0);
    // NVIC_EnableIRQ(HardFault_IRQn);
    // NVIC_SetPriority(BusFault_IRQn, 0);
    // NVIC_EnableIRQ(BusFault_IRQn);
    // NVIC_SetPriority(UsageFault_IRQn, 0);
    // NVIC_EnableIRQ(UsageFault_IRQn);

    myDelay(10);

    printf("CAL set LOW (off)\n");
    myDelay(10);

    /*---------- setup motor ----------*/
    m.tim = TIM1;
    m.spi = SPI1;
    m.enable_pin  = ENABLE_Pin;
    m.enable_port = ENABLE_Port;
    m.cal_pin  = CAL_Pin;
    m.cal_port = CAL_Port;
    m.nfault_pin  = nFAULT_Pin;
    m.nfault_port = nFAULT_Port;
    m.initialised = 0;
    bldc_init_drv(&m);
    /*---------- end setup motor ----------*/

    printf("reading ADC voltages:\n");
    printf("m.ADC_voltage_A: %f\n", bldc_get_phase_voltage(&m.ADC_voltage_A));
    printf("m.ADC_voltage_B: %f\n", bldc_get_phase_voltage(&m.ADC_voltage_B));
    printf("m.ADC_voltage_C: %f\n", bldc_get_phase_voltage(&m.ADC_voltage_C));

    // don't think this is working yet
    transmit_uart(USART3, "test\n", 5);

    /* ---------- test DRV connection ---------- */
    uint16_t rx;
    printf("transmitting to drv\n");
    rx = drv8323_read_reg(&m.drv, DRV_GATE_DRIVE_HS);
    printf("Gate Drive HS register\t");   print_reg(rx, 16);
    if (rx != 0b01000000){
        printf("drv comms failed\n");
        error_handler(45);
    }
    printf("successful response\n");
    /* ---------- end test DRV connection ---------- */

    /* ---------- enable motor ---------- */
    bldc_enable(&m);
    /* -------- end enable motor -------- */
        
    watchdog_reload();

    // ---------- setup main loop ---------- */
    uint16_t t_del = 3000;
    m.fault = 0;
    printf(">> while(1)\n");
    // -------- end setup main loop -------- */

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
            
            bldc_update_foc_params(&m);
            bldc_update(&m);

            watchdog_reload();
        }
    }
    /* ------------------------------ MAIN LOOP END -------------------------------- */
}

void error_handler(uint8_t f){
    printf("checking...\n");
    printf("error code %d encountered!\n", f);
    while(1){
        blink_fault();
        watchdog_reload();

        if(f!=45){
            printf("%d != %d\n", f, 45);
            watchdog_reload();
        } else {
            printf(".");
        }
    }
}

void blink_fault(void){
    for(int i = 0; i < 2; i++){
        // pin_set(LED_R_Port,   LED_R_Pin);
        // pin_set(LED_G_Port,   LED_G_Pin);
        pin_set(LED_B_Port,   LED_B_Pin);
        myDelay(300);
        // pin_reset(LED_R_Port,   LED_R_Pin);
        // pin_reset(LED_G_Port,   LED_G_Pin);
        pin_reset(LED_B_Port,   LED_B_Pin);
        myDelay(300);
    }
}