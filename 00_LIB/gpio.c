#include <stdio.h>
#include "gpio.h"

GPIO_Driver_T *m_gpio_drv = NULL;

int32_t gpio_register_driver(GPIO_Driver_T *gpio_driver)
{ 
    m_gpio_drv = gpio_driver; 
    
    return 0;
}

int32_t gpio_init(uint8_t pin, uint8_t mode)
{
    if(m_gpio_drv == NULL) {
        return -1;
    }
    
    return m_gpio_drv->gpio_init(pin, mode);
}

int32_t gpio_write(uint8_t pin, uint8_t value)
{
    if(m_gpio_drv == NULL) {
        return -1;
    }
    
    return m_gpio_drv->gpio_write(pin, value);
}

uint8_t gpio_read(uint8_t pin)
{
    if(m_gpio_drv == NULL) {
        return 0xFF;
    }

    return m_gpio_drv->gpio_read(pin);    
}
