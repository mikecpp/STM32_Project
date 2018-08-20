#include "stm32f429_fire.h"

void BSP_LED_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    
    LED_RED_GPIO_CLK_ENABLE();
    LED_GREEN_GPIO_CLK_ENABLE();
    LED_BLUE_GPIO_CLK_ENABLE();
    LED_4_GPIO_CLK_ENABLE();
    
    GPIO_InitStruct.Pin     = LED_RED_PIN | LED_GREEN_PIN | LED_BLUE_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_PULLUP;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    HAL_GPIO_Init(LED_RED_GPIO_PORT,   &GPIO_InitStruct);
    HAL_GPIO_Init(LED_GREEN_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_Init(LED_BLUE_GPIO_PORT,  &GPIO_InitStruct);

    GPIO_InitStruct.Pin     = LED_4_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_PULLUP;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    HAL_GPIO_Init(LED_4_GPIO_PORT,   &GPIO_InitStruct);
    
    HAL_GPIO_WritePin(LED_RED_GPIO_PORT,   LED_RED_PIN,   GPIO_PIN_SET); 
    HAL_GPIO_WritePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_BLUE_GPIO_PORT,  LED_BLUE_PIN,  GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_4_GPIO_PORT,     LED_4_PIN,     GPIO_PIN_SET);    
}

void BSP_LED_On(LED_T led)
{
    switch(led) {
        case LED_RED: 
             HAL_GPIO_WritePin(LED_RED_GPIO_PORT, LED_RED_PIN, GPIO_PIN_RESET);
             break;
        case LED_GREEN: 
             HAL_GPIO_WritePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
             break;
        case LED_BLUE:
             HAL_GPIO_WritePin(LED_BLUE_GPIO_PORT, LED_BLUE_PIN, GPIO_PIN_RESET); 
             break;
        case LED_4:
             HAL_GPIO_WritePin(LED_4_GPIO_PORT, LED_4_PIN, GPIO_PIN_RESET);
             break;
        default:
             break;
    }
} 

void BSP_LED_Off(LED_T led)
{
    switch(led) {
        case LED_RED: 
             HAL_GPIO_WritePin(LED_RED_GPIO_PORT, LED_RED_PIN, GPIO_PIN_SET);
             break;
        case LED_GREEN: 
             HAL_GPIO_WritePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
             break;
        case LED_BLUE:
             HAL_GPIO_WritePin(LED_BLUE_GPIO_PORT, LED_BLUE_PIN, GPIO_PIN_SET); 
             break;
        case LED_4:
             HAL_GPIO_WritePin(LED_4_GPIO_PORT, LED_4_PIN, GPIO_PIN_SET);
             break;
        default:
             break;
    }
}

void BSP_LED_Toggle(LED_T led)
{
    switch(led) {
        case LED_RED: 
             HAL_GPIO_TogglePin(LED_RED_GPIO_PORT, LED_RED_PIN);
             break;
        case LED_GREEN: 
             HAL_GPIO_TogglePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN);
             break;
        case LED_BLUE:
             HAL_GPIO_TogglePin(LED_BLUE_GPIO_PORT, LED_BLUE_PIN);
             break;
        case LED_4:
             HAL_GPIO_TogglePin(LED_4_GPIO_PORT, LED_4_PIN);
             break;
        default:
             break;
    }    
}

void BSP_BUTTON_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    
    BUTTON_UP_GPIO_CLK_ENABLE();
    BUTTON_DOWN_GPIO_CLK_ENABLE();
    
    GPIO_InitStruct.Pin   = BUTTON_UP_PIN;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(BUTTON_UP_GPIO_PORT, &GPIO_InitStruct); 
    
    GPIO_InitStruct.Pin   = BUTTON_DOWN_PIN | BUTTON_LEFT_PIN | BUTTON_RIGHT_PIN;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;   
    HAL_GPIO_Init(BUTTON_DOWN_GPIO_PORT, &GPIO_InitStruct); 
    
    HAL_NVIC_SetPriority(BUTTON_UP_EXTI_IRQn,    0x0F, 0);
    HAL_NVIC_EnableIRQ(BUTTON_UP_EXTI_IRQn); 
    
    HAL_NVIC_SetPriority(BUTTON_DOWN_EXTI_IRQn,  0x0F, 1);
    HAL_NVIC_EnableIRQ(BUTTON_DOWN_EXTI_IRQn); 
    
    HAL_NVIC_SetPriority(BUTTON_LEFT_EXTI_IRQn,  0x0F, 2);
    HAL_NVIC_EnableIRQ(BUTTON_LEFT_EXTI_IRQn); 
    
    HAL_NVIC_SetPriority(BUTTON_RIGHT_EXTI_IRQn, 0x0F, 3);
    HAL_NVIC_EnableIRQ(BUTTON_RIGHT_EXTI_IRQn);     
}

void BSP_BEEP_Init(void)
{
}

void BSP_BEEP_On(void)
{
}

void BSP_BEEP_Off(void)
{
}

void BSP_BEEP_Toggle(void)
{
}
