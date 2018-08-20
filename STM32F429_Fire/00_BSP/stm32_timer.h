#ifndef __STM32_TIMER_H__
#define __STM32_TIMER_H__

#include "main.h"

#define TIMER_ID            TIM2
#define TIMER_CLK_ENABLE()  __HAL_RCC_TIM2_CLK_ENABLE()
#define TIMER_IRQ           TIM2_IRQn

#ifdef USE_STM32412G_DISCOVERY
    #define TIMER_PRESCALER     (100 - 1)
#endif

#ifdef USE_STM32F723E_DISCOVERY
    #define TIMER_PRESCALER     (108 - 1)
#endif

#define TIMER_PERIOD        (1000 - 1)

typedef void (*hw_timer_cb)(void);

int32_t stm32_timer_init(hw_timer_cb func);
int32_t stm32_timer_entry(void);
int32_t stm32_timer_start(void);
int32_t stm32_timer_stop(void);
int32_t stm32_timer_delayus(uint16_t delay);

#endif//__STM32_TIMER_H__
