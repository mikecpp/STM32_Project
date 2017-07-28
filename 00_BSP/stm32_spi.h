#ifndef __STM32_SPI_H__
#define __STM32_SPI_H__

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_spi.h"
#include "spi.h"

// SPI_1 (A1, A5, B4, B5)
#define SPI1_CLK_ENABLE()                    __HAL_RCC_SPI1_CLK_ENABLE()

#define SPI1_NSS_GPIO_PORT                   GPIOA
#define SPI1_NSS_PIN                         GPIO_PIN_1
#define SPI1_NSS_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_NSS_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define SPI1_NSS_AF                          GPIO_AF5_SPI1

#define SPI1_SCK_GPIO_PORT                   GPIOA
#define SPI1_SCK_PIN                         GPIO_PIN_5
#define SPI1_SCK_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_SCK_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define SPI1_SCK_AF                          GPIO_AF5_SPI1

#define SPI1_MISO_MOSI_GPIO_PORT             GPIOB
#define SPI1_MISO_MOSI_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI1_MISO_MOSI_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
#define SPI1_MISO_PIN                        GPIO_PIN_4
#define SPI1_MOSI_PIN                        GPIO_PIN_5
#define SPI1_MISO_AF                         GPIO_AF5_SPI1
#define SPI1_MOSI_AF                         GPIO_AF5_SPI1

// SPI_2 (A11, A12, B14, B15)
#define SPI2_CLK_ENABLE()                    __HAL_RCC_SPI2_CLK_ENABLE()

#define SPI2_NSS_GPIO_PORT                   GPIOA
#define SPI2_NSS_PIN                         GPIO_PIN_11
#define SPI2_NSS_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI2_NSS_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define SPI2_NSS_AF                          GPIO_AF5_SPI2

#define SPI2_SCK_GPIO_PORT                   GPIOA
#define SPI2_SCK_PIN                         GPIO_PIN_12
#define SPI2_SCK_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI2_SCK_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define SPI2_SCK_AF                          GPIO_AF5_SPI2

#define SPI2_MISO_MOSI_GPIO_PORT             GPIOB
#define SPI2_MISO_MOSI_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_MISO_MOSI_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
#define SPI2_MISO_PIN                        GPIO_PIN_14
#define SPI2_MOSI_PIN                        GPIO_PIN_15
#define SPI2_MISO_AF                         GPIO_AF5_SPI2
#define SPI2_MOSI_AF                         GPIO_AF5_SPI2

int32_t stm32_spi_init(uint8_t id);
int32_t stm32_spi_write_read(uint8_t id, uint8_t *pWrite, uint8_t *pRead, uint16_t len);
int32_t stm32_spi_write(uint8_t id, uint8_t *pData, uint16_t len);

extern SPI_Driver_T stm32_spi_drv;

#endif//__STM32F103_SPI_H__
