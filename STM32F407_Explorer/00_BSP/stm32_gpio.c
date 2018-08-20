#include "stm32_gpio.h"

GPIO_Driver_T stm32_gpio_drv =  
{
    .gpio_init  = stm32_gpio_init,
    .gpio_write = stm32_gpio_write,
    .gpio_read  = stm32_gpio_read,
};

typedef struct {
    GPIO_TypeDef* port;
    uint8_t       pin;
} GPIO_Pin_T;

GPIO_Pin_T gpio_pin[] = {
    {GPIOA, 3},     // D0
    {GPIOA, 2},     // D1
    {GPIOC, 5},     // D2
    {GPIOE, 5},     // D3
    {GPIOH, 3},     // D4
    {GPIOB, 0},     // D5
    {GPIOE, 6},     // D6
    {GPIOE, 3},     // D7
    
    {GPIOE, 4},     // D8
    {GPIOH, 6},     // D9
    {GPIOA, 1},     // D10
    {GPIOB, 5},     // D11
    {GPIOB, 4},     // D12
    {GPIOA, 5},     // D13
};  

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

int32_t stm32_gpio_init(uint8_t pin, uint8_t mode)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    gpio_enable_clock(gpio_pin[pin].port);
    
    GPIO_InitStruct.Pin       = 0x01 << gpio_pin[pin].pin; 
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  
    if(mode == GPIO_INPUT) {  
        GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    }
    else { 
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    }
 
    HAL_GPIO_Init(gpio_pin[pin].port, &GPIO_InitStruct); 
    
    return 0;
}

int32_t stm32_gpio_write(uint8_t pin, uint8_t value)
{
    if(value == GPIO_LOW) {    
        HAL_GPIO_WritePin(gpio_pin[pin].port, 0x01 << gpio_pin[pin].pin, GPIO_PIN_RESET);
    }
    else {
        HAL_GPIO_WritePin(gpio_pin[pin].port, 0x01 << gpio_pin[pin].pin, GPIO_PIN_SET);
    }
    
    return 0;
}

uint8_t stm32_gpio_read(uint8_t pin)
{
    return HAL_GPIO_ReadPin(gpio_pin[pin].port, 0x01 << gpio_pin[pin].pin);
}
