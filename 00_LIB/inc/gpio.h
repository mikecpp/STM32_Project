#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f4xx.h"

#define GPIO_TYPE_INPUT     0
#define GPIO_TYPE_OUTPUT    1

void gpio_init(GPIO_TypeDef* port, uint16_t pin, uint8_t type);
void gpio_write(GPIO_TypeDef* port, uint16_t pin, uint8_t value);
uint8_t gpio_read(GPIO_TypeDef* port, uint16_t pin);
void gpio_toggle(GPIO_TypeDef* port, uint16_t pin);

#endif//__GPIO_H__
