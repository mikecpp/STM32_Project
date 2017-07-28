#include "adc.h"

#define ADC_MAX_NUM     3

ADC_HandleTypeDef m_adc_handle[ADC_MAX_NUM];

ADC_HandleTypeDef* adc_handle(uint8_t id)
{
    ADC_HandleTypeDef *p_adc_handle = NULL; 
    
    switch(id) {
        case ADC_1:
            p_adc_handle = &m_adc_handle[0]; 
            p_adc_handle->Instance = ADC1;  
            break;
        case ADC_2:
            p_adc_handle = &m_adc_handle[1];
            p_adc_handle->Instance = ADC2;
            break;
        case ADC_3:
            p_adc_handle = &m_adc_handle[2]; 
            p_adc_handle->Instance = ADC3;  
            break;
        default: 
            return NULL;
    }
       
    return p_adc_handle;
}

int adc_init(ADC_ID id)
{
    ADC_HandleTypeDef *p_adc_handle = adc_handle(id);
    ADC_ChannelConfTypeDef sConfig;
    
    p_adc_handle->Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    p_adc_handle->Init.ScanConvMode          = ADC_SCAN_ENABLE;
    p_adc_handle->Init.ContinuousConvMode    = ENABLE;
    p_adc_handle->Init.NbrOfConversion       = 1;
    p_adc_handle->Init.DiscontinuousConvMode = DISABLE;
    p_adc_handle->Init.NbrOfDiscConversion   = 1;
    p_adc_handle->Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    
    if (HAL_ADC_Init(p_adc_handle) != HAL_OK) {
        return -1;
    }    
    
    sConfig.Channel      = ADC_CHANNEL_0;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
    
    if (HAL_ADC_ConfigChannel(p_adc_handle, &sConfig) != HAL_OK)
    {
        return -1;
    }
  
    HAL_ADC_Start(p_adc_handle); 
    
    return 0;
}

int adc_read(ADC_ID id)
{
    int value = 0;
    
    ADC_HandleTypeDef *p_adc_handle = adc_handle(id);

    if(HAL_ADC_PollForConversion(p_adc_handle, 1000000) == HAL_OK) {
        value = HAL_ADC_GetValue(p_adc_handle);
    }    
    
    return value;
}
