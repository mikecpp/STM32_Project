#ifndef __ADC_H__
#define __ADC_H__

#include "main.h"

#define ARDUINO_A0      (1)
#define ARDUINO_A1      (11)
#define ARDUINO_A2      (13)
#define ARDUINO_A3      (14)
#define ARDUINO_A4      (15)
#define ARDUINO_A5      (8)

int32_t adc_init(uint8_t ch_id);
int32_t adc_start(uint8_t ch_id);
int32_t adc_stop(uint8_t ch_id);
int32_t adc_read(uint8_t ch_id, uint16_t *value);

#endif//__ADC_H__
