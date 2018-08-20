#ifndef __DHT11_H__
#define __DHT11_H__

#ifdef STM32L476xx
    #include "stm32l4xx_hal.h"
    #include "stm32l476_system.h"
    #define DHT11_GPIO_PIN               GPIO_PIN_0 // PB0
    #define DHT11_GPIO_PORT              GPIOB
    #define DHT11_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
    #define DHT11_PIN                    0    
#endif

#ifdef STM32F103xE
    #include "stm32f1xx_hal.h"
    #include "stm32f103_system.h"
    #define DHT11_GPIO_PIN               GPIO_PIN_0 // PB0
    #define DHT11_GPIO_PORT              GPIOB
    #define DHT11_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
    #define DHT11_PIN                    0
#endif

#ifdef STM32F412Zx
    #include "stm32f4xx_hal.h"
    #include "stm32f412_system.h"    
    #define DHT11_GPIO_PIN               GPIO_PIN_13 // PG13 (Arduino-D2)
    #define DHT11_GPIO_PORT              GPIOG
    #define DHT11_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOG_CLK_ENABLE()
    #define DHT11_PIN                    13
#endif

#ifdef STM32F429xx
    #include "stm32f4xx_hal.h"
    #include "stm32f429_system.h"    
    #define DHT11_GPIO_PIN               GPIO_PIN_0 // PB0
    #define DHT11_GPIO_PORT              GPIOB
    #define DHT11_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
    #define DHT11_PIN                    0    
#endif

typedef enum {
    DHT_11,
    DHT_22
} DHT_TYPE;

int DHT11_init(DHT_TYPE type);
int DHT11_read(uint16_t *temp, uint16_t *humi);

#endif//__DHT11_H__
