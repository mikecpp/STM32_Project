#include "stm32_spi.h"

#define SPI_MAX_NUM         2
#define SPI_TIMEOUT         10000    // 1 second

SPI_HandleTypeDef m_spi_handle[SPI_MAX_NUM];

SPI_Driver_T stm32_spi_drv = 
{
    .spi_init       = stm32_spi_init,
    .spi_write_read = stm32_spi_write_read,
    .spi_write      = stm32_spi_write,
};

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

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
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{

}

SPI_HandleTypeDef* spi_handle(uint8_t id)
{
    SPI_HandleTypeDef *p_spi_handle = NULL; 
    
    switch(id) {
        case SPI_1: 
            p_spi_handle = &m_spi_handle[0]; 
            p_spi_handle->Instance = SPI1;  
            break;
        case SPI_2:
            p_spi_handle = &m_spi_handle[1];
            p_spi_handle->Instance = SPI2;
            break;
        default: 
            return NULL;
    }
       
    return p_spi_handle;
}

void spi_ss_low(uint8_t id)
{
    if(id == SPI_1) {
        HAL_GPIO_WritePin(SPI1_NSS_GPIO_PORT, SPI1_NSS_PIN, GPIO_PIN_RESET);    
    }
    
    if(id == SPI_2) {
        HAL_GPIO_WritePin(SPI2_NSS_GPIO_PORT, SPI2_NSS_PIN, GPIO_PIN_RESET);           
    }
}

void spi_ss_high(uint8_t id)
{
    if(id == SPI_1) {
        HAL_GPIO_WritePin(SPI1_NSS_GPIO_PORT, SPI1_NSS_PIN, GPIO_PIN_SET);    
    }
    
    if(id == SPI_2) {
        HAL_GPIO_WritePin(SPI2_NSS_GPIO_PORT, SPI2_NSS_PIN, GPIO_PIN_SET);           
    }    
}

//////////////////////////////////////////////////////////////////////////////
// SPI
//
int32_t stm32_spi_init(uint8_t id)
{
    SPI_HandleTypeDef *p_spi_handle = spi_handle(id);    
    
    p_spi_handle->Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_8; 
    p_spi_handle->Init.Direction          = SPI_DIRECTION_2LINES;
    p_spi_handle->Init.CLKPhase           = SPI_PHASE_1EDGE;
    p_spi_handle->Init.CLKPolarity        = SPI_POLARITY_LOW; 
    p_spi_handle->Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
    p_spi_handle->Init.CRCPolynomial      = 7;
    p_spi_handle->Init.DataSize           = SPI_DATASIZE_8BIT;
    p_spi_handle->Init.FirstBit           = SPI_FIRSTBIT_MSB;
    p_spi_handle->Init.NSS                = SPI_NSS_SOFT;
    p_spi_handle->Init.TIMode             = SPI_TIMODE_DISABLE;
    p_spi_handle->Init.Mode               = SPI_MODE_MASTER;
    
    if(HAL_SPI_Init(p_spi_handle) != HAL_OK) {
        return -1;
    }    
    
    return 0;
}

int32_t stm32_spi_write_read(uint8_t id, uint8_t *pWrite, uint8_t *pRead, uint16_t len)
{
    HAL_StatusTypeDef status = HAL_OK;
    SPI_HandleTypeDef *p_spi_handle = spi_handle(id);    
    
    spi_ss_low(id);
  
    status = HAL_SPI_TransmitReceive(p_spi_handle, pWrite, pRead, len, SPI_TIMEOUT);

    spi_ss_high(id);
    
    if(status != HAL_OK) {
        return -1;
    }    
    
    return 0;
}

int32_t stm32_spi_write(uint8_t id, uint8_t *pData, uint16_t len)
{
    HAL_StatusTypeDef status = HAL_OK;
    SPI_HandleTypeDef *p_spi_handle = spi_handle(id);    

    spi_ss_low(id);
    
    status = HAL_SPI_Transmit(p_spi_handle, pData, len, SPI_TIMEOUT);

    spi_ss_high(id);
    
    if(status != HAL_OK) {
        return -1;
    }    
    
    return 0;
}
