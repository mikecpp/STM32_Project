#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_adc.h"

typedef enum {
    ADC_1,
    ADC_2,
    ADC_3
} ADC_ID;

int adc_init(ADC_ID id);
int adc_read(ADC_ID id);

#endif//__ADC_H__
