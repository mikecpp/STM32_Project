#include "stm32f407_explorer.h"
#include "stm32f4xx_ll_fsmc.h"

void BSP_LED_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    
    LED_GREEN_GPIO_CLK_ENABLE();
    LED_RED_GPIO_CLK_ENABLE();
    
    GPIO_InitStruct.Pin     = LED_RED_PIN | LED_GREEN_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_PULLUP;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    
    HAL_GPIO_Init(LED_RED_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_Init(LED_GREEN_GPIO_PORT, &GPIO_InitStruct);
    
    HAL_GPIO_WritePin(LED_RED_GPIO_PORT, LED_RED_PIN, GPIO_PIN_SET); 
    HAL_GPIO_WritePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
}

void BSP_LED_On(LED_T led)
{
    if(led == LED_RED) {
        HAL_GPIO_WritePin(LED_RED_GPIO_PORT, LED_RED_PIN, GPIO_PIN_RESET);
    }

    if(led == LED_GREEN) { 
        HAL_GPIO_WritePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
    }
} 

void BSP_LED_Off(LED_T led)
{
    if(led == LED_RED) {
        HAL_GPIO_WritePin(LED_RED_GPIO_PORT, LED_RED_PIN, GPIO_PIN_SET);
    }

    if(led == LED_GREEN) { 
        HAL_GPIO_WritePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
    }
}

void BSP_LED_Toggle(LED_T led)
{
    if(led == LED_RED) {
        HAL_GPIO_TogglePin(LED_RED_GPIO_PORT, LED_RED_PIN);
    }

    if(led == LED_GREEN) { 
        HAL_GPIO_TogglePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN);
    }    
}

void BSP_BUTTON_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    
    BUTTON_UP_GPIO_CLK_ENABLE();
    BUTTON_DOWN_GPIO_CLK_ENABLE();
    
    GPIO_InitStruct.Pin   = BUTTON_UP_PIN;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(BUTTON_UP_GPIO_PORT, &GPIO_InitStruct); 
    
    GPIO_InitStruct.Pin   = BUTTON_DOWN_PIN | BUTTON_LEFT_PIN | BUTTON_RIGHT_PIN;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;   
    HAL_GPIO_Init(BUTTON_DOWN_GPIO_PORT, &GPIO_InitStruct); 
    
    HAL_NVIC_SetPriority(BUTTON_UP_EXTI_IRQn,    0x0F, 0);
    HAL_NVIC_EnableIRQ(BUTTON_UP_EXTI_IRQn); 
    
    HAL_NVIC_SetPriority(BUTTON_DOWN_EXTI_IRQn,  0x0F, 1);
    HAL_NVIC_EnableIRQ(BUTTON_DOWN_EXTI_IRQn); 
    
    HAL_NVIC_SetPriority(BUTTON_LEFT_EXTI_IRQn,  0x0F, 2);
    HAL_NVIC_EnableIRQ(BUTTON_LEFT_EXTI_IRQn); 
    
    HAL_NVIC_SetPriority(BUTTON_RIGHT_EXTI_IRQn, 0x0F, 3);
    HAL_NVIC_EnableIRQ(BUTTON_RIGHT_EXTI_IRQn);     
}

void BSP_BEEP_Init(void)
{
}

void BSP_BEEP_On(void)
{
}

void BSP_BEEP_Off(void)
{
}

void BSP_BEEP_Toggle(void)
{
}

//////////////////////////////////////////////////////////////////////////////
// FSMC

static void FSMC_MspInit(void)
{
    GPIO_InitTypeDef gpio_init_structure;
    
    /* Enable FSMC clock */
    __HAL_RCC_FSMC_CLK_ENABLE();
  
    /* Enable GPIOs clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    
    // Back Light
    gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
#if defined(USE_STM32F407_MINI)    
    gpio_init_structure.Pin       = GPIO_PIN_1;  // B1
#else // USE_STM32F407_EXPLORER
    gpio_init_structure.Pin       = GPIO_PIN_15; // B15
#endif    
    gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Pull      = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &gpio_init_structure);    
    
    /* Common GPIO configuration */
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Pull      = GPIO_PULLUP;
    gpio_init_structure.Alternate = GPIO_AF12_FSMC; 
 
    // GPIOD configuration 
#if defined(USE_STM32F407_MINI)    
    gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1  | GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_7  |
                                GPIO_PIN_8 | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14 | 
                                GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &gpio_init_structure);
#endif

#if defined(USE_STM32F407_EXPLORER)
    gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1  | GPIO_PIN_4  | GPIO_PIN_5 | GPIO_PIN_8 |
                                GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &gpio_init_structure);
#endif

    // GPIOE configuration 
    gpio_init_structure.Pin   = GPIO_PIN_7  | GPIO_PIN_8 | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11 |
                                GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &gpio_init_structure); 
    
#if defined(USE_STM32F407_EXPLORER)
    // GPIOF Configuration 
    gpio_init_structure.Pin   = GPIO_PIN_12; 
    HAL_GPIO_Init(GPIOF, &gpio_init_structure); 

    // GPIOG Configuration 
    gpio_init_structure.Pin   = GPIO_PIN_12; 
    HAL_GPIO_Init(GPIOG, &gpio_init_structure); 
#endif //USE_STM32F407_EXPLORER
}

void BSP_FSMC_Init(void) 
{  
    SRAM_HandleTypeDef hsram;
    FSMC_NORSRAM_TimingTypeDef sram_timing_read;
    FSMC_NORSRAM_TimingTypeDef sram_timing_write;

    // Timing for READING 
    sram_timing_read.AddressSetupTime           = 0x0F;
    sram_timing_read.AddressHoldTime            = 0x00;
    sram_timing_read.DataSetupTime              = 0x3C; // 60Hz
    sram_timing_read.BusTurnAroundDuration      = 0x00;
    sram_timing_read.CLKDivision                = 0x00;
    sram_timing_read.DataLatency                = 0x00;
    sram_timing_read.AccessMode                 = FSMC_ACCESS_MODE_A; 

    // Timing for WRITTING
    sram_timing_write.AddressSetupTime          = 0x09;
    sram_timing_write.AddressHoldTime           = 0x00;
    sram_timing_write.DataSetupTime             = 0x08;
    sram_timing_write.BusTurnAroundDuration     = 0x00;
    sram_timing_write.CLKDivision               = 0x00;
    sram_timing_write.DataLatency               = 0x00;
    sram_timing_write.AccessMode                = FSMC_ACCESS_MODE_A;

    hsram.Instance                = FSMC_NORSRAM_DEVICE;
    hsram.Extended                = FSMC_NORSRAM_EXTENDED_DEVICE;
    
    hsram.Init.NSBank             = FSMC_NORSRAM_BANK4;
    hsram.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
    hsram.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
    hsram.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
    hsram.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;    
    hsram.Init.ExtendedMode       = FSMC_EXTENDED_MODE_ENABLE;
    hsram.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;

    FSMC_MspInit();
    if(HAL_SRAM_Init(&hsram, &sram_timing_read, &sram_timing_write) != HAL_OK) {
        printf("HAL_SRAM_Init() Fail !\r\n");
    }        
}
