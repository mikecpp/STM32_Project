#include "gpio.h"

static void gpio_enable_clock(GPIO_TypeDef* port)
{
    if(port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if(port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if(port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    else if(port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
    else if(port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }
    else if(port == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    }
    else if(port == GPIOG) {
        __HAL_RCC_GPIOG_CLK_ENABLE();
    }
    else if(port == GPIOH) {
        __HAL_RCC_GPIOH_CLK_ENABLE();
    }   
}

void gpio_init(GPIO_TypeDef* port, uint16_t pin, uint8_t type)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    gpio_enable_clock(port);
    
    GPIO_InitStruct.Pin       = 0x01 << pin; 
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

    printf("PIN = %X \r\n", GPIO_InitStruct.Pin);
    
    if(type == GPIO_TYPE_INPUT) {
        GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    }
    else { // GPIO_TYPE_OUTPUT
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    }
 
    HAL_GPIO_Init(port, &GPIO_InitStruct); 
}

void gpio_write(GPIO_TypeDef* port, uint16_t pin, uint8_t value)
{
    HAL_GPIO_WritePin(port, 0x01 << pin, value);
}

uint8_t gpio_read(GPIO_TypeDef* port, uint16_t pin)
{
    return HAL_GPIO_ReadPin(port, 0x01 << pin);
}

void gpio_toggle(GPIO_TypeDef* port, uint16_t pin)
{
    HAL_GPIO_TogglePin(port, 0x01 << pin);
}
