#ifndef __STM32F412_I2C_H__
#define __STM32F412_I2C_H__

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

// I2C1 (SCL: PB6, SDA: PB7)
#define I2C1_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define I2C1_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C1_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2C1_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define I2C1_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

#define I2C1_SCL_PIN                    GPIO_PIN_6      // PB6
#define I2C1_SCL_GPIO_PORT              GPIOB
#define I2C1_SCL_AF                     GPIO_AF4_I2C1

#define I2C1_SDA_PIN                    GPIO_PIN_7      // PB7
#define I2C1_SDA_GPIO_PORT              GPIOB
#define I2C1_SDA_AF                     GPIO_AF4_I2C1

// I2C2 (SCL: PB10, SDA: PB9)
#define I2C2_CLK_ENABLE()               __HAL_RCC_I2C2_CLK_ENABLE()
#define I2C2_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C2_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2C2_FORCE_RESET()              __HAL_RCC_I2C2_FORCE_RESET()
#define I2C2_RELEASE_RESET()            __HAL_RCC_I2C2_RELEASE_RESET()

#define I2C2_SCL_PIN                    GPIO_PIN_10       // PB10
#define I2C2_SCL_GPIO_PORT              GPIOB
#define I2C2_SCL_AF                     GPIO_AF4_I2C2

#define I2C2_SDA_PIN                    GPIO_PIN_9        // PB9
#define I2C2_SDA_GPIO_PORT              GPIOB
#define I2C2_SDA_AF                     GPIO_AF9_I2C2

#endif//__STM32F412_I2C_H__
