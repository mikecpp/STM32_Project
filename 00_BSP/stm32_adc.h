#ifndef __STM32_ADC_H__
#define __STM32_ADC_H__

#include "main.h"

#define ARDUINO_A0      (0)
#define ARDUINO_A1      (11)
#define ARDUINO_A2      (13)
#define ARDUINO_A3      (14)
#define ARDUINO_A4      (15)
#define ARDUINO_A5      (8)

typedef struct {
    uint8_t         ch_id;
    uint16_t        pin;
    GPIO_TypeDef*   port; 
    uint32_t        channel;
} ADC_CH_T;

int32_t stm32_adc_init(uint8_t ch_id); 
int32_t stm32_adc_start(uint8_t ch_id);
int32_t stm32_adc_stop(uint8_t ch_id);
int32_t stm32_adc_get_value(uint8_t ch_id, uint16_t *value);

#endif//__STM32_ADC_H__
