#ifndef __STM32F469_SYSTEM_H__
#define __STM32F469_SYSTEM_H__

#ifdef __cplusplus
 extern "C" {
#endif
     
#include "stm32f4xx_hal.h"

int system_init(void);
void delay_ms(uint32_t millis);

#ifdef __cplusplus
}
#endif

#endif//_SYSTEM_H__
