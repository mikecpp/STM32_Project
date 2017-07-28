#ifndef __STM32F428_FIRE_H__
#define __STM32F428_FIRE_H__

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif
     
typedef enum {
    LED_1,
    LED_2,
    LED_3,
    LED_4, 
    LED_RED   = LED_1, 
    LED_GREEN = LED_2,
    LED_BLUE  = LED_3
} LED_T;

typedef enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT
} BUTTON_T;

// LED PIN Definition
#define LED_RED_PIN                         GPIO_PIN_10
#define LED_RED_GPIO_PORT                   GPIOH
#define LED_RED_GPIO_CLK_ENABLE()           __GPIOH_CLK_ENABLE()
#define LED_RED_GPIO_CLK_DISABLE()          __GPIOH_CLK_DISABLE()

#define LED_GREEN_PIN                       GPIO_PIN_11
#define LED_GREEN_GPIO_PORT                 GPIOH
#define LED_GREEN_GPIO_CLK_ENABLE()         __GPIOH_CLK_ENABLE()
#define LED_GREEN_GPIO_CLK_DISABLE()        __GPIOH_CLK_DISABLE()

#define LED_BLUE_PIN                         GPIO_PIN_12
#define LED_BLUE_GPIO_PORT                   GPIOH
#define LED_BLUE_GPIO_CLK_ENABLE()           __GPIOH_CLK_ENABLE()
#define LED_BLUE_GPIO_CLK_DISABLE()          __GPIOH_CLK_DISABLE()

#define LED_4_PIN                           GPIO_PIN_11
#define LED_4_GPIO_PORT                     GPIOD
#define LED_4_GPIO_CLK_ENABLE()             __GPIOD_CLK_ENABLE()
#define LED_4_GPIO_CLK_DISABLE()            __GPIOD_CLK_DISABLE()

// BUTTON PIN Definition
#define BUTTON_UP_PIN                       GPIO_PIN_0
#define BUTTON_UP_GPIO_PORT                 GPIOA
#define BUTTON_UP_GPIO_CLK_ENABLE()         __GPIOA_CLK_ENABLE()
#define BUTTON_UP_GPIO_CLK_DISABLE()        __GPIOA_CLK_DISABLE()
#define BUTTON_UP_EXTI_IRQn                 EXTI0_IRQn

#define BUTTON_DOWN_PIN                     GPIO_PIN_3
#define BUTTON_DOWN_GPIO_PORT               GPIOE
#define BUTTON_DOWN_GPIO_CLK_ENABLE()       __GPIOE_CLK_ENABLE()
#define BUTTON_DOWN_GPIO_CLK_DISABLE()      __GPIOE_CLK_DISABLE()
#define BUTTON_DOWN_EXTI_IRQn               EXTI3_IRQn

#define BUTTON_LEFT_PIN                     GPIO_PIN_2
#define BUTTON_LEFT_GPIO_PORT               GPIOE
#define BUTTON_LEFT_GPIO_CLK_ENABLE()       __GPIOE_CLK_ENABLE()
#define BUTTON_LEFT_GPIO_CLK_DISABLE()      __GPIOE_CLK_DISABLE()
#define BUTTON_LEFT_EXTI_IRQn               EXTI2_IRQn

#define BUTTON_RIGHT_PIN                    GPIO_PIN_4
#define BUTTON_RIGHT_GPIO_PORT              GPIOE
#define BUTTON_RIGHT_GPIO_CLK_ENABLE()      __GPIOE_CLK_ENABLE()
#define BUTTON_RIGHT_GPIO_CLK_DISABLE()     __GPIOE_CLK_DISABLE()
#define BUTTON_RIGHT_EXTI_IRQn              EXTI4_IRQn

// BEEP PIN Definition
#define BEEP_PIN                            GPIO_PIN_8
#define BEEP_GPIO_PORT                      GPIOF
#define BEEP_GPIO_CLK_ENABLE()              __GPIOF_CLK_ENABLE()
#define BEEP_GPIO_CLK_DISABLE()             __GPIOF_CLK_DISABLE()

#define SD_DETECT_PIN                        GPIO_PIN_3
#define SD_DETECT_GPIO_PORT                  GPIOD
#define SD_DETECT_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOD_CLK_ENABLE()
#define SD_DETECT_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOD_CLK_DISABLE()
#define SD_DETECT_EXTI_IRQn                  EXTI3_IRQn

void BSP_LED_Init(void);
void BSP_LED_On(LED_T led);
void BSP_LED_Off(LED_T led);
void BSP_LED_Toggle(LED_T led);

void BSP_BUTTON_Init(void);

void BSP_BEEP_Init(void);
void BSP_BEEP_On(void);
void BSP_BEEP_Off(void);
void BSP_BEEP_Toggle(void);

#ifdef __cplusplus
}
#endif

#endif//__STM32F429_FIRE_H__
