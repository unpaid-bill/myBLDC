#include "hw_init.h"

void GPIO_Init(void)
{
    /* GPIO Ports Clock Enable */
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // enable AFIO
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // enable GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // enable GPIOB
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // enable GPIOC
    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN; // enable GPIOD
    
    // DRV Pin setup
    // pin_mode(     nOCTW_Port, nOCTW_Pin, GPIO_IN_PULL);
    // pin_pullmode( nOCTW_Port, nOCTW_Pin, GPIO_PULLUP);
    pin_mode(     nFAULT_Port, nFAULT_Pin, GPIO_IN_PULL);
    pin_pullmode( nFAULT_Port, nFAULT_Pin, GPIO_PULLUP);
    pin_mode(CAL_Port,     CAL_Pin,     GPIO_OUT_PP);       pin_reset(CAL_Port, CAL_Pin);
    pin_mode(ENABLE_Port,  ENABLE_Pin,  GPIO_OUT_PP);       pin_reset(ENABLE_Port, ENABLE_Pin);
    pin_mode(INH_A_Port,   INH_A_Pin,   GPIO_AF_PP);        pin_reset(INH_A_Port, INH_A_Pin);
    pin_mode(INH_B_Port,   INH_B_Pin,   GPIO_AF_PP);        pin_reset(INH_B_Port, INH_B_Pin);
    pin_mode(INH_C_Port,   INH_C_Pin,   GPIO_AF_PP);        pin_reset(INH_C_Port, INH_C_Pin);
    pin_mode(INL_A_Port,   INL_A_Pin,   GPIO_AF_PP);        pin_reset(INL_A_Port, INL_A_Pin);
    pin_mode(INL_B_Port,   INL_B_Pin,   GPIO_AF_PP);        pin_reset(INL_B_Port, INL_B_Pin);
    pin_mode(INL_C_Port,   INL_C_Pin,   GPIO_AF_PP);        pin_reset(INL_C_Port, INL_C_Pin);
    pin_mode(LED_FAULT_Port, LED_FAULT_Pin, GPIO_OUT_PP);   pin_reset(LED_FAULT_Port, LED_FAULT_Pin);

    // SPI Pin setup
    pin_mode(SPI_PORT, SPI_Pin_NSS, GPIO_OUT_PP); // NSS
    pin_mode(SPI_PORT, SPI_Pin_SCK, GPIO_AF_PP);  // SCK,
    pin_mode(SPI_PORT, SPI_Pin_MISO, GPIO_IN_FL);  // MISO
    pin_mode(SPI_PORT, SPI_Pin_MOSI, GPIO_AF_PP);  // MOSI

    // setup GPIO pins for USART1
    pin_mode(USART3_PORT, USART_Pin_RX, GPIO_IN_FL); // RX PB7
    pin_mode(USART3_PORT, USART_Pin_TX, GPIO_AF_PP); // TX PB6

    // setup GPIO pins for ADC1
    pin_mode(ASENSE_Port, ASENSE_Pin, GPIO_IN_AN);
    pin_mode(BSENSE_Port, BSENSE_Pin, GPIO_IN_AN);
    pin_mode(CSENSE_Port, CSENSE_Pin, GPIO_IN_AN);
    pin_mode(SOA_Port,    SOA_Pin,    GPIO_IN_AN);
    pin_mode(SOB_Port,    SOB_Pin,    GPIO_IN_AN);
    pin_mode(SOC_Port,    SOC_Pin,    GPIO_IN_AN);
}

/**@brief System Clock Configuration*/
void SystemClock_Config(void)
{
    myTicks = 0; // reset myTicks for SysTick operation

    RCC->CR |= (1 << 0);          // HSI enable
    while(!(RCC->CR & (1<<1)));  // wait for HSI to stabilise
    // calibrate
    // RCC->CFGR &= ~(0b1110 << 18);  // PLLMUL = x16
    RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_PLLMULL16))| RCC_CFGR_PLLMULL16;  // PLLMUL = x16
    RCC->CFGR &= ~RCC_CFGR_PLLSRC;       // PLLSOURCE = HSI/2
    RCC->CR |= (1 << 24);         // PLL enable
    while(!(RCC->CR & (1<<25))); // wait for PLL to stabilise

     // increase FLASH latency before setting SYSCLK > 24MHz
    FLASH->ACR = (FLASH->ACR & (~FLASH_ACR_LATENCY)) | FLASH_ACR_LATENCY_2;

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1; // Set AHB prescaler to SYSLCK/1
    RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_SW_Msk)) | RCC_CFGR_SW_PLL;    // Set SYSCLCK to PLL
    while(!((RCC->CR & RCC_CR_PLLRDY) && (RCC->CR & RCC_CR_HSIRDY)));
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_PPRE1_Msk)) | RCC_CFGR_PPRE1_DIV2;  // APB1 = HCLK/2
    RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_PPRE2_Msk)) | RCC_CFGR_PPRE2_DIV1;  // APB2 = HCLK/1

    // SystemCoreClock = 64000000;
    SystemCoreClockUpdate();
    
    /*Configure the Systick interrupt time*/
    SysTick_Config(64000);
    NVIC_SetPriority(SysTick_IRQn, 0);

}

int __io_putchar(int ch)
{
    // transmit_byte_uart(USART3, (uint8_t)ch);
    ITM_SendChar(ch);
    return ch;
}

void assess_reset_condition(void){
    uint32_t control_status_reg = RCC->CSR;
    if(control_status_reg & RCC_CSR_LPWRRSTF){
        printf("Low-power reset detected\n");
    }
    if(control_status_reg & RCC_CSR_WWDGRSTF){
        printf("Window watchdog reset detected\n");
    }
    if(control_status_reg & RCC_CSR_IWDGRSTF){
        printf("Independent watchdog reset detected\n");
    }
    if(control_status_reg & RCC_CSR_SFTRSTF){
        printf("Software reset detected\n");
    }
    if(control_status_reg & RCC_CSR_PORRSTF){
        printf("POR/PDR reset detected\n");
    }
    if(control_status_reg & RCC_CSR_PINRSTF){
        printf("Pin reset detected\n");
    }

    RCC->CSR |= RCC_CSR_RMVF; // clear reset flags
}