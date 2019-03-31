#ifndef __STM32_I2C_H__
#define __STM32_I2C_H__

#ifdef USE_STM32F723E_DISCOVERY
    #include "stm32f7xx_hal.h"
    #include "stm32f7xx_hal_i2c.h"
#endif

#ifdef USE_STM32412G_DISCOVERY
    #include "stm32f4xx_hal.h"
    #include "stm32f4xx_hal_i2c.h"
#endif

#include "i2c.h"

#ifdef __cplusplus
 extern "C" {
#endif
     
// I2C1 (SCL: PB6, SDA: PB7)
#define I2C1_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define I2C1_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C1_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2C1_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define I2C1_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

#define I2C1_SCL_PIN                    GPIO_PIN_6              // PB6
#define I2C1_SCL_GPIO_PORT              GPIOB
#define I2C1_SCL_AF                     GPIO_AF4_I2C1

#define I2C1_SDA_PIN                    GPIO_PIN_7              // PB7
#define I2C1_SDA_GPIO_PORT              GPIOB
#define I2C1_SDA_AF                     GPIO_AF4_I2C1

#ifdef USE_STM32412G_DISCOVERY
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
#endif

#ifdef USE_STM32F723E_DISCOVERY
    // I2C2 (SCL: PH4, SDA: PH5)
    #define I2C2_CLK_ENABLE()               __HAL_RCC_I2C2_CLK_ENABLE()
    #define I2C2_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOH_CLK_ENABLE()
    #define I2C2_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOH_CLK_ENABLE() 

    #define I2C2_FORCE_RESET()              __HAL_RCC_I2C2_FORCE_RESET()
    #define I2C2_RELEASE_RESET()            __HAL_RCC_I2C2_RELEASE_RESET()

    #define I2C2_SCL_PIN                    GPIO_PIN_4  
    #define I2C2_SCL_GPIO_PORT              GPIOH
    #define I2C2_SCL_AF                     GPIO_AF4_I2C2

    #define I2C2_SDA_PIN                    GPIO_PIN_5       
    #define I2C2_SDA_GPIO_PORT              GPIOH
    #define I2C2_SDA_AF                     GPIO_AF4_I2C2
#endif

int32_t stm32_i2c_init(uint8_t id);
int32_t stm32_i2c_write_byte(uint8_t id, uint8_t addr, uint8_t value);
int32_t stm32_i2c_read_byte(uint8_t id, uint8_t addr, uint8_t *value);
int32_t stm32_i2c_write(uint8_t id, uint8_t addr, uint8_t *buf, int32_t len);
int32_t stm32_i2c_read(uint8_t id, uint8_t addr, uint8_t *buf, int32_t len);

extern I2C_Driver_T stm32_i2c_drv;

#ifdef __cplusplus
}
#endif

#endif//__STM32_I2C_H__
