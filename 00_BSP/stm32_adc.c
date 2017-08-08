#include "stm32_adc.h"

#define MODE_INT    0
#define MODE_DMA    1

ADC_CH_T m_adc_channel[19] = 
{
    0,  0, 0, 0,
    1,  GPIO_PIN_1, GPIOA, ADC_CHANNEL_1,   // A0
    2,  0, 0, 0, 
    3,  0, 0, 0, 
    4,  0, 0, 0, 
    5,  0, 0, 0, 
    6,  0, 0, 0, 
    7,  0, 0, 0,
    8,  GPIO_PIN_0, GPIOB, ADC_CHANNEL_8,   // A5
    9,  0, 0, 0, 
    10, 0, 0, 0, 
    11, GPIO_PIN_1, GPIOC, ADC_CHANNEL_11,  // A1
    12, GPIO_PIN_2, GPIOC, ADC_CHANNEL_12,  
    13, GPIO_PIN_3, GPIOC, ADC_CHANNEL_13,  // A2
    14, GPIO_PIN_4, GPIOC, ADC_CHANNEL_14,  // A3
    15, GPIO_PIN_5, GPIOC, ADC_CHANNEL_15,  // A4
    16, 0, 0, 0, 
    17, 0, 0, 0, 
    18, 0, 0, 0
};  

ADC_HandleTypeDef       AdcHandle;
ADC_ChannelConfTypeDef  AdcConfig;
uint8_t m_ch_id = 1;

uint16_t AdcValue = 255;

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    GPIO_InitTypeDef          GPIO_InitStruct;
    static DMA_HandleTypeDef  hdma_adc;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
    
    GPIO_InitStruct.Pin  = m_adc_channel[m_ch_id].pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(m_adc_channel[m_ch_id].port, &GPIO_InitStruct);

#if MODE_INT    
    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);    
#endif    

#if MODE_DMA
    hdma_adc.Instance                 = DMA2_Stream0;
    hdma_adc.Init.Channel             = DMA_CHANNEL_0;
    hdma_adc.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_adc.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_adc.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hdma_adc.Init.Mode                = DMA_CIRCULAR;
    hdma_adc.Init.Priority            = DMA_PRIORITY_HIGH;
    hdma_adc.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_adc.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_HALFFULL;
    hdma_adc.Init.MemBurst            = DMA_MBURST_SINGLE;
    hdma_adc.Init.PeriphBurst         = DMA_PBURST_SINGLE;   
    
    HAL_DMA_Init(&hdma_adc);

    __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc); 

    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);    
#endif    
}

int32_t stm32_adc_init(uint8_t ch_id)
{   
    m_ch_id = ch_id;
    
    AdcHandle.Instance                   = ADC1;
  
    AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
    AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;

    AdcHandle.Init.ScanConvMode          = DISABLE;  /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
    AdcHandle.Init.ContinuousConvMode    = ENABLE;   /* Continuous mode disabled to have only 1 conversion at each conversion trig */
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;  /* Parameter discarded because sequencer is disabled */
    AdcHandle.Init.NbrOfDiscConversion   = 0;
    AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Conversion start trigged at each external event */
    AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.NbrOfConversion       = 1;
#if MODE_DMA    
    AdcHandle.Init.DMAContinuousRequests = ENABLE;
#endif
#if MODE_INT    
    AdcHandle.Init.DMAContinuousRequests = DISABLE;
#endif    
    AdcHandle.Init.EOCSelection          = DISABLE;    
    
    if(HAL_ADC_Init(&AdcHandle) != HAL_OK) {
        return -1;
    }

    AdcConfig.Channel      = m_adc_channel[ch_id].channel;
    AdcConfig.Rank         = 1;
    AdcConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
    AdcConfig.Offset       = 0;

    if (HAL_ADC_ConfigChannel(&AdcHandle, &AdcConfig) != HAL_OK) {
        return -1;
    }    
 
    return 0; 
}

int32_t stm32_adc_start(uint8_t ch_id)
{
#if MODE_DMA    
    if(HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)&AdcValue, 1) != HAL_OK) {
        return -1;
    }      
#endif

#if MODE_INT
    if (HAL_ADC_Start_IT(&AdcHandle) != HAL_OK) {
        return -1;
    }
#endif
    
    return 0;
}

int32_t stm32_adc_stop(uint8_t ch_id)
{
#if MODE_DMA        
    HAL_ADC_Stop_DMA(&AdcHandle);
#endif

#if MODE_INT  
    HAL_ADC_Stop_IT(&AdcHandle);
#endif    
    
    return 0;
}

int32_t stm32_adc_read(uint8_t ch_id, uint16_t *value)
{      
    *value = AdcValue;
    
    return 0;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
    AdcValue = HAL_ADC_GetValue(AdcHandle);
}


void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    printf("Error !!! \r\n");
}
