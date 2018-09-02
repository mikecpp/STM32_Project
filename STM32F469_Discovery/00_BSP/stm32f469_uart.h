#ifndef __STM32F469_UART_H__
#define __STM32F469_UART_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
extern UART_HandleTypeDef huart3;
extern void _Error_Handler(char *, int);
void MX_USART3_UART_Init(void);

#ifdef __cplusplus
}
#endif
#endif
