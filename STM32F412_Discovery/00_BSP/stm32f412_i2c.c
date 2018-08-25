#include "stm32f412_i2c.h"

#define STM32F412_I2C_1
#define STM32F412_I2C_2

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
#ifdef STM32F412_I2C_1
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
#endif
    
#ifdef STM32F412_I2C_2
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
#endif

}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
#ifdef STM32F412_I2C_1  
    I2C1_FORCE_RESET();
    I2C1_RELEASE_RESET();

    HAL_GPIO_DeInit(I2C1_SCL_GPIO_PORT, I2C1_SCL_PIN);
    HAL_GPIO_DeInit(I2C1_SDA_GPIO_PORT, I2C1_SDA_PIN);
#endif    

#ifdef STM32F412_I2C_2
    I2C2_FORCE_RESET();
    I2C2_RELEASE_RESET();

    HAL_GPIO_DeInit(I2C2_SCL_GPIO_PORT, I2C2_SCL_PIN);
    HAL_GPIO_DeInit(I2C2_SDA_GPIO_PORT, I2C2_SDA_PIN);
#endif    
}
