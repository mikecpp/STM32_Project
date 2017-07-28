#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "stm32f7xx_hal.h"

int system_init(void);
void delay_ms(uint32_t millis);
void CPU_CACHE_Enable(void);
void MPU_ConfigPSRAM(void);

#endif//__SYSTEM_H__
