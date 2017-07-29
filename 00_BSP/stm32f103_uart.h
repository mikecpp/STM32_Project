#ifndef __STM32F103_UART_H__
#define __STM32F103_UART_H__

// USART-1 (TX: PA9, RX: PA10)
#define USART1_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define USART1_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART1_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USART1_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USART1_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

// Definition for USARTx Pins 
#define USART1_TX_PIN                    GPIO_PIN_9
#define USART1_TX_GPIO_PORT              GPIOA
#define USART1_RX_PIN                    GPIO_PIN_10
#define USART1_RX_GPIO_PORT              GPIOA

// USART-2 (TX: PA2, RX: PA3) 
#define USART2_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
#define USART2_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART2_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USART2_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USART2_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

// Definition for USARTx Pins 
#define USART2_TX_PIN                    GPIO_PIN_2
#define USART2_TX_GPIO_PORT              GPIOA
#define USART2_RX_PIN                    GPIO_PIN_3
#define USART2_RX_GPIO_PORT              GPIOA

#endif//__STM32F103_UART_H__
