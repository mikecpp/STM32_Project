#ifndef __STM32_ADC_H__
#define __STM32_ADC_H__

#include "main.h"

typedef struct {
    uint8_t         ch_id;
    uint16_t        pin;
    GPIO_TypeDef*   port; 
    uint32_t        channel;
} ADC_CH_T;

int32_t stm32_adc_init(uint8_t ch_id); 
int32_t stm32_adc_start(uint8_t ch_id);
int32_t stm32_adc_stop(uint8_t ch_id);
int32_t stm32_adc_read(uint8_t ch_id, uint16_t *value);

#endif//__STM32_ADC_H__
