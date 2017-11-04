#ifndef __UART_H__
#define __UART_H__

#ifdef STM32L476xx
    #include "stm32l4xx_hal.h"
    #include "stm32l4xx_hal_uart.h"
#endif

#ifdef USE_STM32F103_FIRE
    #include "stm32f1xx_hal.h"
    #include "stm32f1xx_hal_uart.h"
#endif

#if defined(USE_STM32412G_DISCOVERY) || defined(USE_STM32F429_FIRE) || \
    defined(USE_STM32F429I_DISCO)    || defined(USE_STM32F407_MINI) || \
    defined(USE_STM32F407_EXPLORER) 
    #include "stm32f4xx_hal.h"
    #include "stm32f4xx_hal_uart.h"
#endif

#ifdef USE_STM32F723E_DISCOVERY
    #include "stm32f7xx_hal.h"
    #include "stm32f7xx_hal_uart.h"
#endif

#ifdef __cplusplus
 extern "C" {
#endif
     
typedef enum {
    UART_1,
    UART_2,
    UART_3,
    UART_6,    
} UART_ID;    

int uart_init(UART_ID id, uint32_t baud_rate);
int uart_consol(UART_ID id);

int uart_send(UART_ID id, uint8_t *buf, uint16_t len);
int uart_recv(UART_ID id, uint8_t *buf, uint16_t len);
char uart_getchar(UART_ID id);
int uart_putchar(UART_ID id, char ch);
int uart_available(UART_ID id);
int uart_clean(UART_ID id);

#ifdef __cplusplus
}
#endif

#endif//__UART_H__
