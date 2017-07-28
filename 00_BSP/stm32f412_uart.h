#ifndef __STM32F412_UART_H__
#define __STM32F412_UART_H__
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

// USART2 (TX: A2, RX: A3) 
#define USART2_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USART2_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART2_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USART2_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USART2_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

#define USART2_TX_PIN                    GPIO_PIN_2
#define USART2_TX_GPIO_PORT              GPIOA
#define USART2_TX_AF                     GPIO_AF7_USART2
#define USART2_RX_PIN                    GPIO_PIN_3
#define USART2_RX_GPIO_PORT              GPIOA
#define USART2_RX_AF                     GPIO_AF7_USART2

// USART6 (TX:G14, RX: G9)
#define USART6_CLK_ENABLE()              __HAL_RCC_USART6_CLK_ENABLE();
#define USART6_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOG_CLK_ENABLE()
#define USART6_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOG_CLK_ENABLE()

#define USART6_FORCE_RESET()             __HAL_RCC_USART6_FORCE_RESET()
#define USART6_RELEASE_RESET()           __HAL_RCC_USART6_RELEASE_RESET()

#define USART6_TX_PIN                    GPIO_PIN_14
#define USART6_TX_GPIO_PORT              GPIOG
#define USART6_TX_AF                     GPIO_AF8_USART6
#define USART6_RX_PIN                    GPIO_PIN_9
#define USART6_RX_GPIO_PORT              GPIOG
#define USART6_RX_AF                     GPIO_AF8_USART6

#endif//__STM32F412_UART_H__
