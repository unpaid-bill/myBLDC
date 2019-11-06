#include "iwdg.h"



void watchdog_reload(void){
    IWDG->KR = 0xAAAA;
}

void watchdog_start(void){
    IWDG->KR = 0xCCCC;
}

void watchdog_set_prescaler(uint16_t pr_val){

    IWDG->KR = 0x5555; // unlock access to PR


    while(IWDG->SR & IWDG_SR_PVU);

    switch (pr_val)
    {
    case 4:
        IWDG->PR = IWDG_PR_DIV_4;
        break;
    case 8:
        IWDG->PR = IWDG_PR_DIV_8;
        break;
    case 16:
        IWDG->PR = IWDG_PR_DIV_16;
        break;
    case 32:
        IWDG->PR = IWDG_PR_DIV_32;
        break;
    case 64:
        IWDG->PR = IWDG_PR_DIV_64;
        break;
    case 128:
        IWDG->PR = IWDG_PR_DIV_128;
        break;
    case 256:
        IWDG->PR = IWDG_PR_DIV_256;
        break;
    default:
        IWDG->PR = IWDG_PR_DIV_4;
        break;
    }
}

void watchdog_set_reload_reg(uint16_t rr_val){
    rr_val &= 0xfff; //trim to 11 bits

    IWDG->KR = 0x5555; // unlock access to PR

    while(IWDG->SR & IWDG_SR_RVU);

    IWDG->RLR = rr_val;

}