#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef STM32F103xE
    #include "stm32f1xx_hal.h"
    #include "stm32f1xx_hal_tim.h"
    #define TIMER_PRESCALER     71
#endif

#ifdef STM32F412Zx
    #include "stm32f4xx_hal.h"
    #include "stm32f4xx_hal_tim.h"
    #define TIMER_PRESCALER     99
#endif

#ifdef STM32F429xx
    #include "stm32f4xx_hal.h"
    #include "stm32f4xx_hal_tim.h"
    #define TIMER_PRESCALER     179
#endif    

#define TIM2_CLK_ENABLE()     __HAL_RCC_TIM2_CLK_ENABLE()

int  timer_init(void);
void timer_start(void);
void timer_stop(void);
void timer_delay_us(const uint16_t delay);

#endif//__TIMER_H__
