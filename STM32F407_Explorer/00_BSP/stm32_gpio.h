#ifndef __STM32_GPIO_H__
#define __STM32_GPIO_H__

#include "stm32f7xx.h"
#include "gpio.h"

int32_t stm32_gpio_init(uint8_t pin, uint8_t mode); 
int32_t stm32_gpio_write(uint8_t pin, uint8_t value);
uint8_t stm32_gpio_read(uint8_t pin);

extern GPIO_Driver_T stm32_gpio_drv;

#endif//__STM32_GPIO_H__
