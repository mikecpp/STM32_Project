#ifndef __STM32F103_FIRE_H
#define __STM32F103_FIRE_H

#include "stm32f1xx_hal.h"
   
typedef enum {
    LED_1, 
    LED_2,
    LED_3, 
    LED_GREEN = LED_1,
    LED_BLUE  = LED_2,
    LED_RED   = LED_3
} Led_TypeDef;

typedef enum {  
    BUTTON_1,
    BUTTON_2
} Button_TypeDef;

typedef enum {
    BUTTON_MODE_GPIO = 0,
    BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef; 

#if !defined (USE_STM32F103_FIRE_H)
 #define USE_STM32F103_FIRE_H
#endif

// LED Pin Define
#define LEDn                             3

#define LED1_PIN                         GPIO_PIN_0
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()  

#define LED2_PIN                         GPIO_PIN_1
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()  

#define LED3_PIN                         GPIO_PIN_5
#define LED3_GPIO_PORT                   GPIOB
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()  

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)   do { if((__INDEX__) == 0) LED2_GPIO_CLK_ENABLE();} while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  (((__INDEX__) == 0) ? LED2_GPIO_CLK_DISABLE() : 0)

// Button in Define
#define BUTTONn                          2

#define BUTTON1_PIN                      GPIO_PIN_0
#define BUTTON1_GPIO_PORT                GPIOA
#define BUTTON1_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUTTON1_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUTTON1_EXTI_IRQn                EXTI0_IRQn

#define BUTTON2_PIN                      GPIO_PIN_13
#define BUTTON2_GPIO_PORT                GPIOC
#define BUTTON2_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUTTON2_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUTTON2_EXTI_IRQn                EXTI15_10_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)    do { if((__INDEX__) == 0) BUTTON1_GPIO_CLK_ENABLE(); BUTTON2_GPIO_CLK_ENABLE();} while(0)
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)   (((__INDEX__) == 0) ? BUTTON1_GPIO_CLK_DISABLE() : 0)

/*###################### SPI1 ###################################*/
#define NUCLEO_SPIx                                 SPI1
#define NUCLEO_SPIx_CLK_ENABLE()                    __HAL_RCC_SPI1_CLK_ENABLE()

#define NUCLEO_SPIx_SCK_GPIO_PORT                   GPIOA
#define NUCLEO_SPIx_SCK_PIN                         GPIO_PIN_5
#define NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define NUCLEO_SPIx_SCK_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

#define NUCLEO_SPIx_MISO_MOSI_GPIO_PORT             GPIOA
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define NUCLEO_SPIx_MISO_PIN                        GPIO_PIN_6
#define NUCLEO_SPIx_MOSI_PIN                        GPIO_PIN_7
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define NUCLEO_SPIx_TIMEOUT_MAX                   1000


/**
  * @brief  SD Control Lines management
  */  
#define SD_CS_LOW()       HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_RESET)
#define SD_CS_HIGH()      HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_SET)
    
/**
  * @brief  LCD Control Lines management
  */
#define LCD_CS_LOW()      HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()     HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_DC_LOW()      HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_HIGH()     HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_SET)

/**
  * @brief  SD Control Interface pins (shield D4)
  */
#define SD_CS_PIN                                 GPIO_PIN_5
#define SD_CS_GPIO_PORT                           GPIOB
#define SD_CS_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOB_CLK_ENABLE()
#define SD_CS_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOB_CLK_DISABLE()

/**
  * @brief  LCD Control Interface pins (shield D10)
  */
#define LCD_CS_PIN                                 GPIO_PIN_6
#define LCD_CS_GPIO_PORT                           GPIOB
#define LCD_CS_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOB_CLK_ENABLE()
#define LCD_CS_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOB_CLK_DISABLE()

/**
  * @brief  LCD Data/Command Interface pins
  */
#define LCD_DC_PIN                                 GPIO_PIN_9
#define LCD_DC_GPIO_PORT                           GPIOA
#define LCD_DC_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_DC_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOA_CLK_DISABLE()
     
uint32_t BSP_GetVersion(void);

void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_DeInit(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);

void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
void BSP_PB_DeInit(Button_TypeDef Button);
uint32_t BSP_PB_GetState(Button_TypeDef Button);

#endif // __STM32F102_FIRE
