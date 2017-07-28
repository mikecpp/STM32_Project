#include "timer.h"

TIM_HandleTypeDef    TimHandle;

int timer_init(void)
{
    TIM2_CLK_ENABLE(); 
    
    TimHandle.Instance               = TIM2;

    TimHandle.Init.Period            = 0xFFFF;
    TimHandle.Init.Prescaler         = TIMER_PRESCALER; 
    TimHandle.Init.ClockDivision     = 0;  
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    TimHandle.Init.RepetitionCounter = 0;

    if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK) {
        return -1;
    }
    
    HAL_TIM_Base_Start(&TimHandle);
    
    return 0;
}

void timer_start(void)
{
    HAL_TIM_Base_Start(&TimHandle);
}

void timer_stop(void)
{
    HAL_TIM_Base_Stop(&TimHandle);
}

void timer_delay_us(const uint16_t delay)
{
    __HAL_TIM_SET_COUNTER(&TimHandle, 0);
    while(__HAL_TIM_GET_COUNTER(&TimHandle) <= delay);
}
