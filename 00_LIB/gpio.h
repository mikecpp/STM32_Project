#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>

#define GPIO_INPUT     1
#define GPIO_OUTPUT    2

#define GPIO_HIGH     1
#define GPIO_LOW      0

typedef struct {
    int32_t (*gpio_init) (uint8_t pin, uint8_t mode);
    int32_t (*gpio_write)(uint8_t pin, uint8_t value);
    uint8_t (*gpio_read) (uint8_t pin); 
} GPIO_Driver_T; 

int32_t gpio_register_driver(GPIO_Driver_T *gpio_driver);
int32_t gpio_init(uint8_t pin, uint8_t mode);
int32_t gpio_write(uint8_t pin, uint8_t value);
uint8_t gpio_read(uint8_t pin);

#endif//__GPIO_H__
