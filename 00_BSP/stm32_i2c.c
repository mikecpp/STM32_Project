#include <stdio.h>
#include "stm32_i2c.h"

#define I2C_MAX_NUM         4
#define I2C_ADDRESS         0x3C << 1
#define I2C_TIMING          0x00E0257A      // 400K Hz
#define I2C_TIMEOUT         10000           // 1 second

I2C_HandleTypeDef m_i2c_handle[I2C_MAX_NUM];

I2C_Driver_T stm32_i2c_drv = 
{
    stm32_i2c_init,
    stm32_i2c_write_byte,
    stm32_i2c_read_byte,
    stm32_i2c_write,
    stm32_i2c_read
};

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    I2C1_SCL_GPIO_CLK_ENABLE(); 
    I2C1_SDA_GPIO_CLK_ENABLE();

    I2C1_CLK_ENABLE(); 

    GPIO_InitStruct.Pin       = I2C1_SCL_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = I2C1_SCL_AF;
    HAL_GPIO_Init(I2C1_SCL_GPIO_PORT, &GPIO_InitStruct);      

    GPIO_InitStruct.Pin       = I2C1_SDA_PIN;
    GPIO_InitStruct.Alternate = I2C1_SDA_AF;
    
    HAL_GPIO_Init(I2C1_SDA_GPIO_PORT, &GPIO_InitStruct);    
    
    I2C2_SCL_GPIO_CLK_ENABLE(); 
    I2C2_SDA_GPIO_CLK_ENABLE();

    I2C2_CLK_ENABLE(); 

    GPIO_InitStruct.Pin       = I2C2_SCL_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = I2C2_SCL_AF;
    HAL_GPIO_Init(I2C2_SCL_GPIO_PORT, &GPIO_InitStruct);      

    GPIO_InitStruct.Pin       = I2C2_SDA_PIN;
    GPIO_InitStruct.Alternate = I2C2_SDA_AF;
    
    HAL_GPIO_Init(I2C2_SDA_GPIO_PORT, &GPIO_InitStruct);    
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    I2C1_FORCE_RESET();
    I2C1_RELEASE_RESET();

    HAL_GPIO_DeInit(I2C1_SCL_GPIO_PORT, I2C1_SCL_PIN);
    HAL_GPIO_DeInit(I2C1_SDA_GPIO_PORT, I2C1_SDA_PIN);

    I2C2_FORCE_RESET();
    I2C2_RELEASE_RESET();

    HAL_GPIO_DeInit(I2C2_SCL_GPIO_PORT, I2C2_SCL_PIN);
    HAL_GPIO_DeInit(I2C2_SDA_GPIO_PORT, I2C2_SDA_PIN);
}

static I2C_HandleTypeDef* i2c_handle(uint8_t id)
{
    I2C_HandleTypeDef *p_i2c_handle = NULL; 
    
    switch(id) {
        case I2C_1:
            p_i2c_handle = &m_i2c_handle[0]; 
            p_i2c_handle->Instance = I2C1;  
            break;
        case I2C_2:
            p_i2c_handle = &m_i2c_handle[1];
            p_i2c_handle->Instance = I2C2;
            break;
        default: 
            return NULL;
    }
       
    return p_i2c_handle;
}

int32_t stm32_i2c_init(uint8_t id)
{
    I2C_HandleTypeDef *p_i2c_handle = i2c_handle(id);

    p_i2c_handle->Init.Timing          = I2C_TIMING; 
    p_i2c_handle->Init.OwnAddress1     = 0x00;
    p_i2c_handle->Init.OwnAddress2     = 0x00;
    p_i2c_handle->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    p_i2c_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    p_i2c_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    p_i2c_handle->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;  
  
    if(HAL_I2C_Init(p_i2c_handle) != HAL_OK) {
        return -1;
    }

    return 0;
}

int32_t stm32_i2c_write_byte(uint8_t id, uint8_t addr, uint8_t value)
{
    I2C_HandleTypeDef *p_i2c_handle = i2c_handle(id);
   
    if(HAL_I2C_Master_Transmit(p_i2c_handle, addr, (uint8_t*) &value, 1, I2C_TIMEOUT) != HAL_OK) {
        return -1;
    }    
    
    return 0;
}

int32_t stm32_i2c_read_byte(uint8_t id, uint8_t addr, uint8_t *value)
{
    I2C_HandleTypeDef *p_i2c_handle = i2c_handle(id);    
    
    if(HAL_I2C_Master_Receive(p_i2c_handle, addr, value, 1, I2C_TIMEOUT) != HAL_OK) {
        return -1;
    }    

    return 0;
}

int32_t stm32_i2c_write(uint8_t id, uint8_t addr, uint8_t *buf, int32_t len)
{
    I2C_HandleTypeDef *p_i2c_handle = i2c_handle(id);    
    uint32_t ret;
    
    if(HAL_I2C_Master_Transmit(p_i2c_handle, addr, buf, len, I2C_TIMEOUT) != HAL_OK) {
        ret = HAL_I2C_GetError(p_i2c_handle);
        return ret;
    }    
    
    return 0; 
}

int32_t stm32_i2c_read(uint8_t id, uint8_t addr, uint8_t *buf, int32_t len) 
{
    I2C_HandleTypeDef *p_i2c_handle = i2c_handle(id);    
    uint32_t ret;   
    
    if(HAL_I2C_Master_Receive(p_i2c_handle, addr, buf, len, I2C_TIMEOUT) != HAL_OK) {
        ret = HAL_I2C_GetError(p_i2c_handle); 
        return ret;
    }
    
    return 0;    
}
