#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"

#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

// Arduino A1
#define ADCx_CHANNEL_PIN                GPIO_PIN_1
#define ADCx_CHANNEL_GPIO_PORT          GPIOC

/* Definition for ADCx's DMA */
#define ADCx_DMA_CHANNEL                DMA_CHANNEL_0
#define ADCx_DMA_STREAM                 DMA2_Stream0

/* Definition for ADCx's NVIC */
#define ADCx_DMA_IRQn                   DMA2_Stream0_IRQn
#define ADCx_DMA_IRQHandler             DMA2_Stream0_IRQHandler

#define ADCx_CHANNEL                    ADC_CHANNEL_11
// #define ADCx_IRQn                       ADC_IRQn

int adc_init(void);
uint16_t adc_get_value(void);

#endif//__ADC_H__
