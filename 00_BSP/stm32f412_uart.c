#include "stm32f412_uart.h"

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    // UART_2
    USART2_TX_GPIO_CLK_ENABLE();
    USART2_RX_GPIO_CLK_ENABLE();

    USART2_CLK_ENABLE();

    GPIO_InitStruct.Pin       = USART2_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = USART2_TX_AF;

    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = USART2_RX_PIN;
    GPIO_InitStruct.Alternate = USART2_RX_AF;

    HAL_GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);    
    
    // UART_6
    USART6_TX_GPIO_CLK_ENABLE();
    USART6_RX_GPIO_CLK_ENABLE();

    USART6_CLK_ENABLE();

    GPIO_InitStruct.Pin       = USART6_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = USART6_TX_AF;

    HAL_GPIO_Init(USART6_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = USART6_RX_PIN;
    GPIO_InitStruct.Alternate = USART6_RX_AF;

    HAL_GPIO_Init(USART6_RX_GPIO_PORT, &GPIO_InitStruct);  
    HAL_NVIC_SetPriority(USART6_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART6_IRQn);    
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    // UART_2
    USART2_FORCE_RESET();
    USART2_RELEASE_RESET();

    HAL_GPIO_DeInit(USART2_TX_GPIO_PORT, USART2_TX_PIN);
    HAL_GPIO_DeInit(USART2_RX_GPIO_PORT, USART2_RX_PIN);

    // UART_6
    USART6_FORCE_RESET();
    USART6_RELEASE_RESET();

    HAL_GPIO_DeInit(USART6_TX_GPIO_PORT, USART6_TX_PIN);
    HAL_GPIO_DeInit(USART6_RX_GPIO_PORT, USART6_RX_PIN);

}
