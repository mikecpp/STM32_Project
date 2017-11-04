#include "adc.h"
#include "stm32_adc.h"

int32_t adc_init(uint8_t ch_id)
{
    return stm32_adc_init(ch_id);    
}
 
int32_t adc_start(uint8_t ch_id)
{
    return stm32_adc_start(ch_id);
}

int32_t adc_stop(uint8_t ch_id)
{
    return stm32_adc_stop(ch_id);
}

int32_t adc_read(uint8_t ch_id, uint16_t *value)
{
    return stm32_adc_read(ch_id, value);
} 
