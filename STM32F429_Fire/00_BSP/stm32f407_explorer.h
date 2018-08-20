#ifndef __STM32F407_EXPLORER_H__
#define __STM32F407_EXPLORER_H__

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

typedef enum {
    LED_RED, 
    LED_GREEN
} LED_T;

typedef enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT
} BUTTON_T;

// LED PIN Definition
#define LED_RED_PIN                         GPIO_PIN_9
#define LED_RED_GPIO_PORT                   GPIOF
#define LED_RED_GPIO_CLK_ENABLE()           __GPIOF_CLK_ENABLE()
#define LED_RED_GPIO_CLK_DISABLE()          __GPIOF_CLK_DISABLE()

#define LED_GREEN_PIN                       GPIO_PIN_10
#define LED_GREEN_GPIO_PORT                 GPIOF
#define LED_GREEN_GPIO_CLK_ENABLE()         __GPIOF_CLK_ENABLE()
#define LED_GREEN_GPIO_CLK_DISABLE()        __GPIOF_CLK_DISABLE()

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

void BSP_LED_Init(void);
void BSP_LED_On(LED_T led);
void BSP_LED_Off(LED_T led);
void BSP_LED_Toggle(LED_T led);

void BSP_BUTTON_Init(void);

void BSP_BEEP_Init(void);
void BSP_BEEP_On(void);
void BSP_BEEP_Off(void);
void BSP_BEEP_Toggle(void);

// FSMC 
void BSP_FSMC_Init(void);

#endif//__STM32F407_EXPLORER_H__
