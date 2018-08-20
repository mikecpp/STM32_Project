#include "stm32_timer.h"

TIM_HandleTypeDef        TimHandle;
hw_timer_cb m_func = 0x00;

int32_t stm32_timer_init(hw_timer_cb func)
{
    m_func = func;
    
    TIMER_CLK_ENABLE();    

    TimHandle.Instance               = TIMER_ID;
    TimHandle.Init.Prescaler         = TIMER_PRESCALER; 
    TimHandle.Init.Period            = TIMER_PERIOD;   
    TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    TimHandle.Init.RepetitionCounter = 0;
    
    if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK) {
        return -1;
    }
    
    HAL_TIM_Base_Start_IT(&TimHandle);
    HAL_NVIC_SetPriority(TIMER_IRQ, 5 ,0);
    
    return 0;
}

int32_t stm32_timer_entry(void)
{
    if(m_func) {
        m_func();
    }    
    
    return 0;
}

int32_t stm32_timer_start(void)
{
    HAL_NVIC_EnableIRQ(TIMER_IRQ);    
    
    return 0;
}

int32_t stm32_timer_stop(void)
{
    HAL_NVIC_DisableIRQ(TIMER_IRQ);    
    
    return 0;
}

int32_t stm32_timer_delayus(uint16_t delay)
{
    static uint32_t last;
    
    // __HAL_TIM_SET_COUNTER(&TimHandle, 0);
    last = __HAL_TIM_GET_COUNTER(&TimHandle);
    while(__HAL_TIM_GET_COUNTER(&TimHandle) - last <= delay);
        
    return 0;
}
