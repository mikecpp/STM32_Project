#include "adc.h" 

ADC_HandleTypeDef    AdcHandle;
__IO uint16_t        uhADCxConvertedValue = 0;

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) 
{
    GPIO_InitTypeDef          GPIO_InitStruct;
    static DMA_HandleTypeDef  hdma_adc;

    ADCx_CLK_ENABLE();
    ADCx_CHANNEL_GPIO_CLK_ENABLE();
    DMAx_CLK_ENABLE();    

    GPIO_InitStruct.Pin     = ADCx_CHANNEL_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
    
    hdma_adc.Instance = ADCx_DMA_STREAM;

    hdma_adc.Init.Channel   = ADCx_DMA_CHANNEL;
    hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc.Init.MemInc    = DMA_MINC_ENABLE;
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc.Init.Mode      = DMA_CIRCULAR;
    hdma_adc.Init.Priority  = DMA_PRIORITY_HIGH;
    hdma_adc.Init.FIFOMode  = DMA_FIFOMODE_DISABLE;
    hdma_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    hdma_adc.Init.MemBurst  = DMA_MBURST_SINGLE;
    hdma_adc.Init.PeriphBurst = DMA_PBURST_SINGLE;

    HAL_DMA_Init(&hdma_adc);

    __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);    

    HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    ADCx_FORCE_RESET();
    ADCx_RELEASE_RESET();

    HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

}

int adc_init(void)
{
    ADC_ChannelConfTypeDef sConfig;    
    
    AdcHandle.Instance                   = ADCx; 
    AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
    AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
    
    AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
    AdcHandle.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode disabled to have only 1 conversion at each conversion trig */
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
    AdcHandle.Init.NbrOfDiscConversion   = 0;
    AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Conversion start trigged at each external event */
    AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.NbrOfConversion       = 1;
    AdcHandle.Init.DMAContinuousRequests = ENABLE;
    AdcHandle.Init.EOCSelection          = DISABLE;

    if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
    {
        return -1;
    }

    sConfig.Channel      = ADC_CHANNEL_1;
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
    sConfig.Offset       = 0;
    
    if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    {
        return -1;
    }
    
    if (HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)&uhADCxConvertedValue, 1) != HAL_OK)
    {
        return -1;
    }
    
    return 0; 
}

uint16_t adc_get_value(void)
{   
    return uhADCxConvertedValue;
}
