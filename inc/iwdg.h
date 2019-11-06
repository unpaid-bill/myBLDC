#ifndef __IWDG_H__
#define __IWDG_H__

#include "stm32f103xb.h"

#define IWDG_PR_DIV_4 0x0
#define IWDG_PR_DIV_8 0x1
#define IWDG_PR_DIV_16 0x2
#define IWDG_PR_DIV_32 0x3
#define IWDG_PR_DIV_64 0x4
#define IWDG_PR_DIV_128 0x5
#define IWDG_PR_DIV_256 0x6

void watchdog_reload(void);
void watchdog_start(void);
void watchdog_set_prescaler(uint16_t pr_val);
void watchdog_set_reload_reg(uint16_t rr_val);

#endif // endif __IWDG_H__