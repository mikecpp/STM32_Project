#ifndef __STM32F412_SPI_H__
#define __STM32F412_SPI_H__

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"

// SPI_1 (A15, A5, A6, A7);
#define SPI1_CLK_ENABLE()                    __HAL_RCC_SPI1_CLK_ENABLE()

#define SPI1_NSS_GPIO_PORT                   GPIOA
#define SPI1_NSS_PIN                         GPIO_PIN_15
#define SPI1_NSS_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_NSS_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define SPI1_NSS_AF                          GPIO_AF5_SPI1

#define SPI1_SCK_GPIO_PORT                   GPIOA
#define SPI1_SCK_PIN                         GPIO_PIN_5
#define SPI1_SCK_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_SCK_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define SPI1_SCK_AF                          GPIO_AF5_SPI1

#define SPI1_MISO_MOSI_GPIO_PORT             GPIOA
#define SPI1_MISO_MOSI_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_MISO_MOSI_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define SPI1_MISO_PIN                        GPIO_PIN_6
#define SPI1_MOSI_PIN                        GPIO_PIN_7
#define SPI1_MISO_AF                         GPIO_AF5_SPI1
#define SPI1_MOSI_AF                         GPIO_AF5_SPI1

// SPI_2 (B12, B13, B14, B15);
#define SPI2_CLK_ENABLE()                    __HAL_RCC_SPI2_CLK_ENABLE()

#define SPI2_NSS_GPIO_PORT                   GPIOB
#define SPI2_NSS_PIN                         GPIO_PIN_12
#define SPI2_NSS_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_NSS_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()
#define SPI2_NSS_AF                          GPIO_AF5_SPI2

#define SPI2_SCK_GPIO_PORT                   GPIOB
#define SPI2_SCK_PIN                         GPIO_PIN_13
#define SPI2_SCK_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_SCK_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()
#define SPI2_SCK_AF                          GPIO_AF5_SPI2

#define SPI2_MISO_MOSI_GPIO_PORT             GPIOB
#define SPI2_MISO_MOSI_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_MISO_MOSI_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
#define SPI2_MISO_PIN                        GPIO_PIN_14
#define SPI2_MOSI_PIN                        GPIO_PIN_15
#define SPI2_MISO_AF                         GPIO_AF5_SPI2
#define SPI2_MOSI_AF                         GPIO_AF5_SPI2

#endif//__STM32F103_SPI_H__
