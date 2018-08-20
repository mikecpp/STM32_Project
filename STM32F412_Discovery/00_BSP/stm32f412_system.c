#include "stm32f412_system.h"
#include "stm32f412_uart.h"

static void SystemClock_Config(void);

int system_init(void)
{
    HAL_Init();

    SystemClock_Config();   
    
    return 0;
}

void delay_ms(uint32_t millis)
{
    HAL_Delay(millis);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 100000000
  *            HCLK(Hz)                       = 100000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 200
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;    
    HAL_StatusTypeDef ret = HAL_OK;

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 200;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    RCC_OscInitStruct.PLL.PLLR = 2;
    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
    if(ret != HAL_OK)
    {
        while(1) { ; } 
    }
    
    /* Select PLLSAI output as USB clock source */
    PeriphClkInitStruct.PLLI2S.PLLI2SM = 8;
    PeriphClkInitStruct.PLLI2S.PLLI2SQ = 4;
    PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CK48;
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CK48CLKSOURCE_PLLI2SQ;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);    

    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
    if(ret != HAL_OK)
    {
        while(1) { ; }  
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    // UART_2
    USART2_TX_GPIO_CLK_ENABLE();
    USART2_RX_GPIO_CLK_ENABLE();

    USART2_CLK_ENABLE();

    GPIO_InitStruct.Pin       = USART2_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = USART2_TX_AF;

    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = USART2_RX_PIN;
    GPIO_InitStruct.Alternate = USART2_RX_AF;

    HAL_GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);    
    
    // UART_6
    USART6_TX_GPIO_CLK_ENABLE();
    USART6_RX_GPIO_CLK_ENABLE();

    USART6_CLK_ENABLE();

    GPIO_InitStruct.Pin       = USART6_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = USART6_TX_AF;

    HAL_GPIO_Init(USART6_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = USART6_RX_PIN;
    GPIO_InitStruct.Alternate = USART6_RX_AF;

    HAL_GPIO_Init(USART6_RX_GPIO_PORT, &GPIO_InitStruct);  
    HAL_NVIC_SetPriority(USART6_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART6_IRQn);    
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    // UART_2
    USART2_FORCE_RESET();
    USART2_RELEASE_RESET();

    HAL_GPIO_DeInit(USART2_TX_GPIO_PORT, USART2_TX_PIN);
    HAL_GPIO_DeInit(USART2_RX_GPIO_PORT, USART2_RX_PIN);

    // UART_6
    USART6_FORCE_RESET();
    USART6_RELEASE_RESET();

    HAL_GPIO_DeInit(USART6_TX_GPIO_PORT, USART6_TX_PIN);
    HAL_GPIO_DeInit(USART6_RX_GPIO_PORT, USART6_RX_PIN);

}
