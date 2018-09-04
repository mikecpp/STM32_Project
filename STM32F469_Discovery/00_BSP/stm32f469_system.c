#include "stm32f469_system.h"

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

static void _Error_Handler(void)
{
    while(1) {}
}

static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 360;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    RCC_OscInitStruct.PLL.PLLR = 6; 
  
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        _Error_Handler();
    }

    if(HAL_PWREx_EnableOverDrive() != HAL_OK) {
        _Error_Handler();
    }
  
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; 
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        _Error_Handler();
    }
    
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);    
}

TIM_HandleTypeDef        htim2; 
uint32_t                 uwIncrementState = 0;

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    RCC_ClkInitTypeDef    clkconfig;
    uint32_t              uwTimclock = 0;
    uint32_t              uwPrescalerValue = 0;
    uint32_t              pFLatency;
  
    HAL_NVIC_SetPriority(TIM2_IRQn, TickPriority ,0); 

    HAL_NVIC_EnableIRQ(TIM2_IRQn); 

    __HAL_RCC_TIM2_CLK_ENABLE();

    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency); 

    uwTimclock = 2*HAL_RCC_GetPCLK1Freq();

    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000) - 1);

    htim2.Instance           = TIM2;
    htim2.Init.Period        = (1000000 / 1000) - 1;
    htim2.Init.Prescaler     = uwPrescalerValue;
    htim2.Init.ClockDivision = 0;
    htim2.Init.CounterMode   = TIM_COUNTERMODE_UP;
  
    if(HAL_TIM_Base_Init(&htim2) == HAL_OK) {
        return HAL_TIM_Base_Start_IT(&htim2);
    }

    return HAL_ERROR;
}

void HAL_SuspendTick(void)
{
    __HAL_TIM_DISABLE_IT(&htim2, TIM_IT_UPDATE);                                                  
}

void HAL_ResumeTick(void)
{
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        HAL_IncTick();
    }
}
