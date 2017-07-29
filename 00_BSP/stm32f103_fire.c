#include "stm32f103_fire.h"
#include "stm32f1xx_ll_fsmc.h"
#include "stm32f1xx_hal_sram.h"

typedef struct {
  __IO uint16_t REG;
  __IO uint16_t RAM;
} LCD_CONTROLLER_TypeDef;

#define __STM32F1XX_NUCLEO_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __STM32F1XX_NUCLEO_BSP_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __STM32F1XX_NUCLEO_BSP_VERSION_SUB2   (0x03) /*!< [15:8]  sub2 version */
#define __STM32F1XX_NUCLEO_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __STM32F1XX_NUCLEO_BSP_VERSION       ((__STM32F1XX_NUCLEO_BSP_VERSION_MAIN << 24)\
                                             |(__STM32F1XX_NUCLEO_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32F1XX_NUCLEO_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32F1XX_NUCLEO_BSP_VERSION_RC))
                                             
#define FMC_BANK1_BASE  ((uint32_t)(0x60000000 | 0x00000000))  
#define FMC_BANK1       ((LCD_CONTROLLER_TypeDef *) FMC_BANK1_BASE)

/**
  * @brief LINK SD Card
  */
#define SD_DUMMY_BYTE            0xFF    
#define SD_NO_RESPONSE_EXPECTED  0x80
   
GPIO_TypeDef*  LED_PORT[LEDn]       = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT};
const uint16_t LED_PIN[LEDn]        = {LED1_PIN, LED2_PIN, LED3_PIN};

GPIO_TypeDef* BUTTON_PORT[BUTTONn]  = {BUTTON1_GPIO_PORT, BUTTON2_GPIO_PORT}; 
const uint16_t BUTTON_PIN[BUTTONn]  = {BUTTON1_PIN, BUTTON2_PIN}; 

const uint8_t  BUTTON_IRQn[BUTTONn] = {BUTTON1_EXTI_IRQn, BUTTON2_EXTI_IRQn};

#ifdef HAL_SPI_MODULE_ENABLED
uint32_t SpixTimeout = NUCLEO_SPIx_TIMEOUT_MAX;        /*<! Value of Timeout when SPI communication fails */
static SPI_HandleTypeDef hnucleo_Spi;
#endif /* HAL_SPI_MODULE_ENABLED */

#ifdef HAL_ADC_MODULE_ENABLED
static ADC_HandleTypeDef hnucleo_Adc;
/* ADC channel configuration structure declaration */
static ADC_ChannelConfTypeDef sConfig;
#endif /* HAL_ADC_MODULE_ENABLED */

#ifdef HAL_SPI_MODULE_ENABLED
static void               SPIx_Init(void);
// static void               SPIx_Write(uint8_t Value);
static void               SPIx_WriteData(uint8_t *DataIn, uint16_t DataLength);
static void               SPIx_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLegnth);
static void               SPIx_Error (void);
static void               SPIx_MspInit(void);
#endif /* HAL_SPI_MODULE_ENABLED */

/* SD IO functions */
void                      SD_IO_Init(void);
void                      SD_IO_CSState(uint8_t state);
void                      SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength);
void                      SD_IO_ReadData(uint8_t *DataOut, uint16_t DataLength);
void                      SD_IO_WriteData(const uint8_t *Data, uint16_t DataLength);
uint8_t                   SD_IO_WriteByte(uint8_t Data);
uint8_t                   SD_IO_ReadByte(void);

// FMC functions
static void               FMC_BANK1_WriteData(uint16_t Data);
static void               FMC_BANK1_WriteReg(uint8_t Reg);
static uint16_t           FMC_BANK1_ReadData(void);
static void               FMC_BANK1_Init(void);
static void               FMC_BANK1_MspInit(void);

// LCD IO functions 
void                      LCD_IO_Init(void);
void                      LCD_IO_WriteData(uint16_t RegValue);
void                      LCD_IO_WriteReg(uint8_t Reg);
void                      LCD_IO_WriteMultipleData(uint16_t *pData, uint32_t Size);
uint16_t                  LCD_IO_ReadData(void);
void                      LCD_IO_Delay(uint32_t Delay);

#ifdef HAL_ADC_MODULE_ENABLED
static HAL_StatusTypeDef  ADCx_Init(void);
static void               ADCx_DeInit(void);
static void               ADCx_MspInit(ADC_HandleTypeDef *hadc);
static void               ADCx_MspDeInit(ADC_HandleTypeDef *hadc);
#endif /* HAL_ADC_MODULE_ENABLED */

uint32_t BSP_GetVersion(void)
{
    return __STM32F1XX_NUCLEO_BSP_VERSION;
}

void BSP_LED_Init(Led_TypeDef Led)
{
    GPIO_InitTypeDef  gpioinitstruct;
  
    /* Enable the GPIO_LED Clock */
    LEDx_GPIO_CLK_ENABLE(Led);

    /* Configure the GPIO_LED pin */
    gpioinitstruct.Pin    = LED_PIN[Led];
    gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpioinitstruct.Pull   = GPIO_NOPULL;
    gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  
    HAL_GPIO_Init(LED_PORT[Led], &gpioinitstruct);
    HAL_GPIO_WritePin(LED_PORT[Led],LED_PIN[Led], GPIO_PIN_SET);
}

void BSP_LED_DeInit(Led_TypeDef Led)
{
      GPIO_InitTypeDef  gpio_init_structure;

    /* Turn off LED */
    HAL_GPIO_WritePin(LED_PORT[Led],LED_PIN[Led], GPIO_PIN_RESET);

    /* DeInit the GPIO_LED pin */
    gpio_init_structure.Pin = LED_PIN[Led];
    HAL_GPIO_DeInit(LED_PORT[Led], gpio_init_structure.Pin);
}

void BSP_LED_On(Led_TypeDef Led)
{
    HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET); 
}

void BSP_LED_Off(Led_TypeDef Led)
{
    HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET); 
}

void BSP_LED_Toggle(Led_TypeDef Led)
{
    HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
}

void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
    GPIO_InitTypeDef gpioinitstruct;

    BUTTONx_GPIO_CLK_ENABLE(Button);

    gpioinitstruct.Pin   = BUTTON_PIN[Button];
    gpioinitstruct.Pull  = GPIO_PULLDOWN;
    gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;

    if (ButtonMode == BUTTON_MODE_GPIO)
    {
        gpioinitstruct.Mode   = GPIO_MODE_INPUT;
        HAL_GPIO_Init(BUTTON_PORT[Button], &gpioinitstruct);
    }
 
    if (ButtonMode == BUTTON_MODE_EXTI)
    {
        gpioinitstruct.Mode   = GPIO_MODE_IT_FALLING; 
        HAL_GPIO_Init(BUTTON_PORT[Button], &gpioinitstruct);

        HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0);
        HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
    }
}

void BSP_PB_DeInit(Button_TypeDef Button)
{
    GPIO_InitTypeDef gpio_init_structure;

    gpio_init_structure.Pin = BUTTON_PIN[Button];
    HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
    HAL_GPIO_DeInit(BUTTON_PORT[Button], gpio_init_structure.Pin);
}

uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
    return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}
  
#ifdef HAL_SPI_MODULE_ENABLED
/******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/
/**
  * @brief  Initialize SPI MSP.
  * @retval None
  */
static void SPIx_MspInit(void)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};
  
  /*** Configure the GPIOs ***/  
  /* Enable GPIO clock */
  NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE();
  NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE();

  /* Configure SPI SCK */
  gpioinitstruct.Pin        = NUCLEO_SPIx_SCK_PIN;
  gpioinitstruct.Mode       = GPIO_MODE_AF_PP;
  gpioinitstruct.Speed      = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(NUCLEO_SPIx_SCK_GPIO_PORT, &gpioinitstruct);

  /* Configure SPI MISO and MOSI */ 
  gpioinitstruct.Pin        = NUCLEO_SPIx_MOSI_PIN;
  HAL_GPIO_Init(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, &gpioinitstruct);
  
  gpioinitstruct.Pin        = NUCLEO_SPIx_MISO_PIN;
  gpioinitstruct.Mode       = GPIO_MODE_INPUT;
  HAL_GPIO_Init(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, &gpioinitstruct);

  /*** Configure the SPI peripheral ***/ 
  /* Enable SPI clock */
  NUCLEO_SPIx_CLK_ENABLE();
}

/**
  * @brief  Initialize SPI HAL.
  * @retval None
  */
static void SPIx_Init(void)
{
  if(HAL_SPI_GetState(&hnucleo_Spi) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    hnucleo_Spi.Instance = NUCLEO_SPIx;
      /* SPI baudrate is set to 8 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 64/8 = 8 MHz) 
       to verify these constraints:
          - ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
            Since the provided driver doesn't use read capability from LCD, only constraint 
            on write baudrate is considered.
          - SD card SPI interface max baudrate is 25MHz for write/read
          - PCLK2 max frequency is 32 MHz 
       */
    hnucleo_Spi.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_8;
    hnucleo_Spi.Init.Direction          = SPI_DIRECTION_2LINES;
    hnucleo_Spi.Init.CLKPhase           = SPI_PHASE_1EDGE;
    hnucleo_Spi.Init.CLKPolarity        = SPI_POLARITY_LOW;
    hnucleo_Spi.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
    hnucleo_Spi.Init.CRCPolynomial      = 7;
    hnucleo_Spi.Init.DataSize           = SPI_DATASIZE_8BIT;
    hnucleo_Spi.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    hnucleo_Spi.Init.NSS                = SPI_NSS_SOFT;
    hnucleo_Spi.Init.TIMode             = SPI_TIMODE_DISABLE;
    hnucleo_Spi.Init.Mode               = SPI_MODE_MASTER;
    
    SPIx_MspInit();
    HAL_SPI_Init(&hnucleo_Spi);
  }
}

/**
  * @brief  SPI Write a byte to device
  * @param  Value: value to be written
  * @retval None
*/
static void SPIx_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_SPI_TransmitReceive(&hnucleo_Spi, (uint8_t*) DataIn, DataOut, DataLength, SpixTimeout);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/**
  * @brief  SPI Write an amount of data to device
  * @param  Value: value to be written
  * @param  DataLength: number of bytes to write
  * @retval None
  */
static void SPIx_WriteData(uint8_t *DataIn, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_Transmit(&hnucleo_Spi, DataIn, DataLength, SpixTimeout);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/*
static void SPIx_Write(uint8_t Value) 
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t data;

  status = HAL_SPI_TransmitReceive(&hnucleo_Spi, (uint8_t*) &Value, &data, 1, SpixTimeout);

  // Check the communication status 
  if(status != HAL_OK)
  {
    // Execute user timeout callback 
    SPIx_Error();
  }
}
*/

/**
  * @brief  SPI error treatment function
  * @retval None
  */
static void SPIx_Error (void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&hnucleo_Spi);

  /* Re-Initiaize the SPI communication BUS */
  SPIx_Init();
}
#endif /* HAL_SPI_MODULE_ENABLED */

/******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/

/********************************* LINK SD ************************************/
/**
  * @brief  Initialize the SD Card and put it into StandBy State (Ready for 
  *         data transfer).
  * @retval None
  */
void SD_IO_Init(void)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};
  uint8_t counter = 0;

  /* SD_CS_GPIO Periph clock enable */
  SD_CS_GPIO_CLK_ENABLE();

  /* Configure SD_CS_PIN pin: SD Card CS pin */
  gpioinitstruct.Pin    = SD_CS_PIN;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_PULLUP;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SD_CS_GPIO_PORT, &gpioinitstruct);

  /* Configure LCD_CS_PIN pin: LCD Card CS pin */
  gpioinitstruct.Pin   = LCD_CS_PIN;
  gpioinitstruct.Mode  = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull  = GPIO_NOPULL;
  gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SD_CS_GPIO_PORT, &gpioinitstruct);
  LCD_CS_HIGH();
  /*------------Put SD in SPI mode--------------*/
  /* SD SPI Config */
  SPIx_Init();

  /* SD chip select high */
  SD_CS_HIGH();
  
  /* Send dummy byte 0xFF, 10 times with CS high */
  /* Rise CS and MOSI for 80 clocks cycles */
  for (counter = 0; counter <= 9; counter++)
  {
    /* Send dummy byte 0xFF */
    SD_IO_WriteByte(SD_DUMMY_BYTE);
  }
}

/**
  * @brief  Set the SD_CS pin.
  * @param  pin value.
  * @retval None
  */
void SD_IO_CSState(uint8_t val)
{
  if(val == 1) 
  {
    SD_CS_HIGH();
}
  else
  {
    SD_CS_LOW();
  }
}
 
/**
  * @brief  Write byte(s) on the SD
  * @param  DataIn: Pointer to data buffer to write
  * @param  DataOut: Pointer to data buffer for read data
  * @param  DataLength: number of bytes to write
  * @retval None
  */
void SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
  {
  /* Send the byte */
  SPIx_WriteReadData(DataIn, DataOut, DataLength);
}

/**
  * @brief  Write a byte on the SD.
  * @param  Data: byte to send.
  * @retval Data written
  */
uint8_t SD_IO_WriteByte(uint8_t Data)
{
  uint8_t tmp;

  /* Send the byte */
  SPIx_WriteReadData(&Data,&tmp,1);
  return tmp;
}

/**
  * @brief  Write an amount of data on the SD.
  * @param  Data: byte to send.
  * @param  DataLength: number of bytes to write
  * @retval none
  */
void SD_IO_ReadData(uint8_t *DataOut, uint16_t DataLength)
{
  /* Send the byte */
  SD_IO_WriteReadData(DataOut, DataOut, DataLength);
  }   
 
/**
  * @brief  Write an amount of data on the SD.
  * @param  Data: byte to send.
  * @param  DataLength: number of bytes to write
  * @retval none
  */
void SD_IO_WriteData(const uint8_t *Data, uint16_t DataLength)
{
  /* Send the byte */
  SPIx_WriteData((uint8_t *)Data, DataLength);
}

/*************************** FMC Routines ************************************/

static void FMC_BANK1_MspInit(void)
{
    GPIO_InitTypeDef gpio_init_structure;
    
    /* Enable FSMC clock */
    __HAL_RCC_FSMC_CLK_ENABLE();
  
    /* Enable GPIOs clock */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
  
    /* Common GPIO configuration */
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
 
    /* GPIOD configuration */ /* GPIO_PIN_7 is  FMC_NE1 */
    gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 |\
                                GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15 |\
                                GPIO_PIN_7 | GPIO_PIN_11;
   
    HAL_GPIO_Init(GPIOD, &gpio_init_structure);

    /* GPIOE configuration */  
    gpio_init_structure.Pin   = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |\
                                GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &gpio_init_structure); 
    
    // RST: PE1
    gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pin   = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOE, &gpio_init_structure);
    
    // BK: PD12
    gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pin   = GPIO_PIN_12;
    HAL_GPIO_Init(GPIOD, &gpio_init_structure);
}

static void FMC_BANK1_Init(void) 
{  
    SRAM_HandleTypeDef hsram;
    FSMC_NORSRAM_TimingTypeDef sram_timing;
    FSMC_NORSRAM_TimingTypeDef sram_timing_write;

    /*** Configure the SRAM Bank 1 ***/
    /* Configure IPs */
    hsram.Instance  = FSMC_NORSRAM_DEVICE;
    hsram.Extended  = FSMC_NORSRAM_EXTENDED_DEVICE;

    /* Timing for READING */
    sram_timing.AddressSetupTime      = 2;
    sram_timing.AddressHoldTime       = 0;
    sram_timing.DataSetupTime         = 5;
    sram_timing.BusTurnAroundDuration = 0;
    sram_timing.CLKDivision           = 0;
    sram_timing.DataLatency           = 0;
    sram_timing.AccessMode            = FSMC_ACCESS_MODE_B; 

    /* Timing for WRITTING*/
    sram_timing_write.AddressSetupTime      = 2;
    sram_timing_write.AddressHoldTime       = 0;
    sram_timing_write.DataSetupTime         = 5;
    sram_timing_write.BusTurnAroundDuration = 0;
    sram_timing_write.CLKDivision           = 0;
    sram_timing_write.DataLatency           = 0;
    sram_timing_write.AccessMode            = FSMC_ACCESS_MODE_B;

    hsram.Init.NSBank             = FSMC_NORSRAM_BANK1;
    hsram.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram.Init.MemoryType         = FSMC_MEMORY_TYPE_NOR;
    hsram.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
    hsram.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
    hsram.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
    hsram.Init.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE;
    hsram.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;

    /* Initialize the SRAM controller */
    FMC_BANK1_MspInit();
    HAL_SRAM_Init(&hsram, &sram_timing, &sram_timing_write);
}

static void FMC_BANK1_WriteData(uint16_t Data) 
{
    /* Write 16-bit Reg */
    FMC_BANK1->RAM = Data;
}

static void FMC_BANK1_WriteReg(uint8_t Reg) 
{
    /* Write 16-bit Index, then write register */
    FMC_BANK1->REG = Reg;
}

static uint16_t FMC_BANK1_ReadData(void) 
{
    return FMC_BANK1->RAM;
}

/********************************* LINK LCD ***********************************/
/**
  * @brief  Initialize the LCD
  * @retval None
  */
void LCD_IO_Init(void)
{
    FMC_BANK1_Init();
}

void LCD_IO_WriteData(uint16_t RegValue)
{
    FMC_BANK1_WriteData(RegValue);
}

void LCD_IO_WriteMultipleData(uint16_t *pData, uint32_t Size)
{
    uint32_t  i;

    for (i = 0; i < Size; i++)
    {
        FMC_BANK1_WriteData(pData[i]);
    }
}

void LCD_IO_WriteReg(uint8_t Reg) 
{
    FMC_BANK1_WriteReg(Reg);
}

uint16_t LCD_IO_ReadData(void) 
{
    return FMC_BANK1_ReadData();
}

void LCD_Delay(uint32_t Delay)
{
    HAL_Delay(Delay);
}
