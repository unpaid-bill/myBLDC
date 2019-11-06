#include "main.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPIO_Init(void);
void blink_fault(void);

int __io_putchar(int ch)
{
    // transmit_byte_uart(USART3, (uint8_t)ch);
    ITM_SendChar(ch);
    return ch;
}

volatile uint32_t myTicks = 0;

void SysTick_Handler(void)
{
    myTicks++;
}

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

void fault(void){
    // TIM1_deInit();
    printf("fault");
}

void ADC1_2_IRQHandler(void){
    // pin_set(GPIOB, 11);
    blink_fault();
    printf("ADC interrupt\n");
}

void HardFault_Handler(void){
    pin_set(LED_FAULT_Port, LED_FAULT_Pin);
}
void BusFault_Handler(void){
    pin_set(LED_FAULT_Port, LED_FAULT_Pin);
}
void UsageFault_Handler(void){
    pin_set(LED_FAULT_Port, LED_FAULT_Pin);
}

void myDelay(uint32_t mS){
    myTicks = 0;
    while(myTicks<mS);
}

uint16_t ADC_samples[10];


void blink_fault(void){
    pin_reset(LED_FAULT_Port, LED_FAULT_Pin);
    myDelay(50);
    pin_set(LED_FAULT_Port, LED_FAULT_Pin);
    myDelay(50);
    pin_reset(LED_FAULT_Port, LED_FAULT_Pin);
    myDelay(50);
    pin_set(LED_FAULT_Port, LED_FAULT_Pin);
}

int main(void)
{   
    // SystemInit();
    // ITM_SendChar('A');
    SystemClock_Config();
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
    // ITM_SendChar('A');



    // pin_reset(ENABLE_Port, ENABLE_Pin);
    // myDelay(50);
    // pin_set(ENABLE_Port, ENABLE_Pin);
    // myDelay(50);
    // pin_reset(ENABLE_Port, ENABLE_Pin);
    // myDelay(50);
    // pin_set(ENABLE_Port, ENABLE_Pin);

    watchdog_set_prescaler(32);
    watchdog_set_reload_reg(0xfff);
    watchdog_start();

    GPIO_Init();
    // for(;;){
    //     myDelay(100);
    // }
    blink_fault();
    UART_Init(USART3);
    // UART_pin_Remap(USART1, 1);
    printf("uart initialized\n");
    printf("gpio initialized\n");
    SPI_Init(SPI1);             printf("SPI1 initialized\n");
    
    if(RCC->CSR & RCC_CSR_IWDGRSTF){
        printf("\nIWDG reset detected\n");
    }
    printf("mem adress: ADC1->DR=%lu\n",(uint32_t)&(ADC1->DR));
    

    NVIC_SetPriority(HardFault_IRQn, 0);
    NVIC_EnableIRQ(HardFault_IRQn);
    NVIC_SetPriority(BusFault_IRQn, 0);
    NVIC_EnableIRQ(BusFault_IRQn);
    NVIC_SetPriority(UsageFault_IRQn, 0);
    NVIC_EnableIRQ(UsageFault_IRQn);

    // printf("RCC->CR\t\t");          print_reg(RCC->CR,     32);

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
    myDelay(10);
    myDelay(500);
    

    TIM1_Init();                printf("tim1 initialized\n");
    pin_reset(CAL_Port, CAL_Pin);
    printf("CAL set LOW (off)\n");
    myDelay(10);
    uint16_t tx;
    uint16_t rx;

    drv8303_TypeDef drv;
    drv8303_Init(&drv, SPI1);
    drv_update(&drv);

    // sets OC_ADJ_SET to 24 (Vds = 1.043v)
    // Disable OCP
    // sets drv to 3-PWM mode
    // uint16_t set_reg = 0b01000000000 | 0b00000110000 | 0b00000001000;
    // uint16_t set_reg = (CR1_OC_ADJ_SET_24 | CR1_OCP_MODE_DISABLED | CR1_PWM_MODE_3);
    uint16_t set_reg = (CR1_OC_ADJ_SET_7 | CR1_PWM_MODE_3);
    pin_set(ENABLE_Port, ENABLE_Pin);
    myDelay(100);
    drv_write(&drv, DRV_CTRL_1, set_reg);
    printf("\nEnabled DRV8303\n");

    transmit_uart(USART3, "test\n", 5);

    // myDelay(10);
    myDelay(500);
    printf("transmitting to drv\n");
    rx = SPI_Transcieve(SPI1, 0b000000000000000);
    print_reg(rx, 16);
    rx = drv_read(&drv, DRV_STATUS_1);
    printf("Status register 1\t");   print_reg(rx, 16);
    if (rx!= 0b0011001100110011){
        printf("error\n");
        while(1){
            blink_fault();
            myDelay(1000);
        }
    }
    rx = drv_read(&drv, DRV_STATUS_2);
    printf("Status register 2\t");   print_reg(rx, 16);
    rx = drv_read(&drv, DRV_CTRL_1);
    printf("Control register 1\t");  print_reg(rx, 16);
    rx = drv_read(&drv, DRV_CTRL_2);
    printf("Control register 2\t");  print_reg(rx, 16);
    myDelay(10);
    // rx = drv_read(0x03);
    // printf("Control register 2\t");  print_reg(rx, 16);
    // myDelay(10);

    printf("\n");
    uint16_t t_del = 50;
    TIM1_enable();


    uint32_t angle_a = 0;
    uint32_t angle_b = 0;
    uint32_t angle_c = 0;
    uint32_t max_duty_cycle = 7500;
    uint16_t duty_cycle_a;
    uint16_t duty_cycle_b;
    uint16_t duty_cycle_c;
    uint16_t angle_step = 30;
    
    // uint8_t tc[3] = "abc";
    // transmit_byte_uart1(*tc);
    // transmit_byte_uart1('\n');
    // transmit_uart(USART1, tc, 3);

    drv_update(&drv);
    
    // drv_write(&drv, DRV_CTRL_1, set_reg);
    watchdog_reload();
    printf("Interrupts enabled\n");

    
    NVIC_SetPriority(DMA1_Channel1_IRQn, 1);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    NVIC_SetPriority(ADC1_IRQn, 2);
    // NVIC_EnableIRQ(ADC1_IRQn);
    printf("while(1)\n");
    while (1){
        // while(1){
        //     ;
        // }
        // myDelay(200);
        // printf("beep..\n");
        // fflush(stdout);
        // ASENSE, BSENSE, CSENSE, A_CURR, B_CURR
        // for(int i = 0; i < 4; i++){
        //     printf("%d, ", ADC_samples[i]);
        // }
        // printf("%d\n", ADC_samples[4]);
        
        // if (DMA1->ISR & DMA_ISR_TEIF1){
        //     printf("DMA1 error interrupt\n");
        //     DMA1->IFCR |= DMA_IFCR_CTEIF1;
        //     DMA1->IFCR |= DMA_IFCR_CGIF1;
        // }
        // if (DMA1->ISR & DMA_ISR_HTIF1){
        //     printf("DMA1 half transfer interrupt\n");
        //     DMA1->IFCR |= DMA_IFCR_CHTIF1;
        //     DMA1->IFCR |= DMA_IFCR_CGIF1;
        // }
        // if (DMA1->ISR & DMA_ISR_TCIF1){
        //     printf("DMA1 transfer complete interrupt\n");
        //     DMA1->IFCR |= DMA_IFCR_CTCIF1;
        //     DMA1->IFCR |= DMA_IFCR_CGIF1;
        // }

        myDelay(10);

        // if(!pin_read(nOCTW_Port, nOCTW_Pin)){
        //     printf("nOCTW is LOW\n");
        //     pin_set(LED_FAULT_Port, LED_FAULT_Pin);
        // } else if(!pin_read(LED_FAULT_Port, nFAULT_Pin)){
        //     printf("nFAULT is LOW\n");
        //     pin_set(LED_FAULT_Port, LED_FAULT_Pin);
        // } else {
        //     pin_reset(LED_FAULT_Port, LED_FAULT_Pin);
        // }

        pin_reset(LED_FAULT_Port, LED_FAULT_Pin);

        angle_b = angle_a + 120;
        angle_c = angle_a + 240;

        duty_cycle_a = ((sin(angle_a * (PI/180))+1)/2)*max_duty_cycle;
        duty_cycle_b = ((sin(angle_b * (PI/180))+1)/2)*max_duty_cycle;
        duty_cycle_c = ((sin(angle_c * (PI/180))+1)/2)*max_duty_cycle;
        // global_angle = fmod(global_angle, 360);
        angle_a =  angle_a + angle_step;
        angle_a %= 360;
        // printf("%d, %d, %d, %d\n", (uint32_t)duty_cycle_a, (uint32_t)duty_cycle_b, (uint32_t)duty_cycle_c, angle_a);

        TIM1->CCR1  = duty_cycle_a;
        TIM1->CCR2  = duty_cycle_b;
        TIM1->CCR3  = duty_cycle_c;

        watchdog_reload();
    }



}

/**@brief System Clock Configuration*/
void SystemClock_Config(void)
{
    
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

static void GPIO_Init(void)
{
    /* GPIO Ports Clock Enable */
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // enable AFIO
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // enable GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // enable GPIOB
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // enable GPIOC
    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN; // enable GPIOD

    // pin_mode(     nOCTW_Port, nOCTW_Pin, GPIO_IN_PULL);
    // pin_pullmode( nOCTW_Port, nOCTW_Pin, GPIO_PULLUP);
    pin_mode(     nFAULT_Port, nFAULT_Pin, GPIO_IN_PULL);
    pin_pullmode( nFAULT_Port, nFAULT_Pin, GPIO_PULLUP);
    
    pin_mode(CAL_Port,  CAL_Pin,  GPIO_OUT_PP);
    pin_mode(ENABLE_Port, ENABLE_Pin, GPIO_OUT_PP);
    pin_mode(INH_A_Port,   INH_A_Pin,   GPIO_AF_PP);
    pin_mode(INH_B_Port,   INH_B_Pin,   GPIO_AF_PP);
    pin_mode(INH_C_Port,   INH_C_Pin,   GPIO_AF_PP);
    pin_mode(INL_A_Port,   INL_A_Pin,   GPIO_AF_PP);
    pin_mode(INL_B_Port,   INL_B_Pin,   GPIO_AF_PP);
    pin_mode(INL_C_Port,   INL_C_Pin,   GPIO_AF_PP);
    pin_mode(LED_FAULT_Port, LED_FAULT_Pin, GPIO_OUT_PP);

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