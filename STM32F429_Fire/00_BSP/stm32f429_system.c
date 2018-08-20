#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "stm32f429_system.h"
#include "stm32f4xx_hal.h"
#include "stm32f429_uart.h"

#define STM32F429_UART_1
#define STM32F429_UART_2

static void SystemClock_Config(void);

int system_init(void)
{
    HAL_Init();

    SystemClock_Config();   
    
    return 0;
}

void delay_ms(uint32_t val)
{
    HAL_Delay(val);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
  
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();
  
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON; 
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
#ifdef USE_STM32F429I_DISCO    
    RCC_OscInitStruct.PLL.PLLM       = 8;
#endif
#ifdef USE_STM32F429_FIRE
    RCC_OscInitStruct.PLL.PLLM       = 25;
#endif    
    RCC_OscInitStruct.PLL.PLLN       = 360;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ       = 7;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        return;
    }

    /* Activate the Over-Drive mode */
    HAL_PWREx_EnableOverDrive();  
    
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; 
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        return;
    }

    /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
    if (HAL_GetREVID() == 0x1001)
    {
        /* Enable the Flash prefetch */
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

#ifdef STM32F429_UART_1 // TX:PA9, RX:PA10
    USART1_TX_GPIO_CLK_ENABLE();
    USART1_RX_GPIO_CLK_ENABLE();

    USART1_CLK_ENABLE();

    GPIO_InitStruct.Pin       = USART1_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = USART1_TX_AF;
    HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = USART1_RX_PIN;
    GPIO_InitStruct.Alternate = USART1_RX_AF;

    HAL_GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStruct);
#endif

#ifdef STM32F429_UART_2 // TX:PA2, RX:PA3
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
#endif
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
#ifdef STM32F429_UART_1
    USART1_FORCE_RESET();
    USART1_RELEASE_RESET();

    HAL_GPIO_DeInit(USART1_TX_GPIO_PORT, USART1_TX_PIN);
    HAL_GPIO_DeInit(USART1_RX_GPIO_PORT, USART1_RX_PIN);
#endif//STM32F429_UART_1
    
#ifdef STM32F429_UART_2
    USART2_FORCE_RESET();
    USART2_RELEASE_RESET();

    HAL_GPIO_DeInit(USART2_TX_GPIO_PORT, USART2_TX_PIN);
    HAL_GPIO_DeInit(USART2_RX_GPIO_PORT, USART2_RX_PIN);
#endif//STM32F429_UART_1
}

#endif//__SYSTEM_H__
