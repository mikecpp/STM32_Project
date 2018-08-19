#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif
     
int system_init(void);
void delay_ms(uint32_t millis);

#ifdef __cplusplus
}
#endif

#endif//_SYSTEM_H__
