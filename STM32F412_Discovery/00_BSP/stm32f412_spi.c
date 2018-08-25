#include "stm32f412_spi.h"

#define STM32F412_SPI_1
#define STM32F412_SPI_2

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

#ifdef STM32F412_SPI_1
    SPI1_NSS_GPIO_CLK_ENABLE();
    SPI1_SCK_GPIO_CLK_ENABLE();
    SPI1_MISO_MOSI_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin        = SPI1_NSS_PIN;
    GPIO_InitStruct.Mode       = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed      = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate  = SPI1_NSS_AF;    
    HAL_GPIO_Init(SPI1_NSS_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin        = SPI1_SCK_PIN;
    GPIO_InitStruct.Mode       = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed      = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate  = SPI1_SCK_AF;        
    HAL_GPIO_Init(SPI1_SCK_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin        = SPI1_MOSI_PIN;
    GPIO_InitStruct.Alternate  = SPI1_MOSI_AF;        
    HAL_GPIO_Init(SPI1_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);
  
    GPIO_InitStruct.Pin        = SPI1_MISO_PIN;
    GPIO_InitStruct.Mode       = GPIO_MODE_INPUT;
    GPIO_InitStruct.Alternate  = SPI1_MISO_AF;        
    HAL_GPIO_Init(SPI1_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);

    SPI1_CLK_ENABLE();  
#endif//STM32F412_SPI_1
    
#ifdef STM32F412_SPI_2   
    SPI2_NSS_GPIO_CLK_ENABLE();
    SPI2_SCK_GPIO_CLK_ENABLE();
    SPI2_MISO_MOSI_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin        = SPI2_NSS_PIN;
    GPIO_InitStruct.Mode       = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed      = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate  = SPI2_NSS_AF;        
    HAL_GPIO_Init(SPI2_NSS_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin        = SPI2_SCK_PIN;
    GPIO_InitStruct.Mode       = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed      = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate  = SPI2_SCK_AF;            
    HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin        = SPI2_MOSI_PIN;
    GPIO_InitStruct.Alternate  = SPI2_MOSI_AF;            
    HAL_GPIO_Init(SPI2_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);
  
    GPIO_InitStruct.Pin        = SPI2_MISO_PIN;
    GPIO_InitStruct.Mode       = GPIO_MODE_INPUT;
    GPIO_InitStruct.Alternate  = SPI2_MISO_AF;       
    HAL_GPIO_Init(SPI2_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);

    SPI2_CLK_ENABLE();   
#endif//STM32F412_SPI_2    
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{

}
