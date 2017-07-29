#include "ili9341.h"
#include "stm32f1xx.h"

LCD_DrvTypeDef ili9341_drv = 
{
    ili9341_Init,
    ili9341_ReadID,
    ili9341_DisplayOn,
    ili9341_DisplayOff,
    ili9341_SetCursor,
    ili9341_WritePixel,
    ili9341_ReadPixel,
    ili9341_SetDisplayWindow,
    ili9341_DrawHLine,
    ili9341_DrawVLine,
    ili9341_GetLcdPixelWidth,
    ili9341_GetLcdPixelHeight,
    ili9341_DrawBitmap,
    ili9341_DrawRGBImage
};

#define      FSMC_Addr_ILI9341_CMD         ( ( uint32_t ) 0x60000000 )	
#define      FSMC_Addr_ILI9341_DATA        ( ( uint32_t ) 0x60020000 )   
#define      CMD_SetCoordinateX		      0x2A	 
#define      CMD_SetCoordinateY		 	  0x2B	 
#define      CMD_SetPixel		 		  0x2C	

// Reset PIN: PE1
#define      ILI9341_RST_PORT               GPIOE           // RST: PE1
#define      ILI9341_RST_PIN                GPIO_PIN_1

#ifdef FIRE_V1
  #define      ILI9341_BK_PORT              GPIOB           // BK_LIGHT: PB1
  #define      ILI9341_BK_PIN               GPIO_PIN_1
#else
  #define      ILI9341_BK_PORT              GPIOD           // BK_LIGHT: PD12
  #define      ILI9341_BK_PIN               GPIO_PIN_12
#endif


__inline void ILI9341_Write_Cmd(uint16_t LCD_Cmd) 
{
	*(volatile uint16_t *)(FSMC_Addr_ILI9341_CMD) = LCD_Cmd;
}

__inline void ILI9341_Write_Data(uint16_t LCD_Data)
{
    *(volatile uint16_t *)(FSMC_Addr_ILI9341_DATA) = LCD_Data;
}

__inline uint16_t ILI9341_Read_Data(void)
{
	return (*(volatile uint16_t*)(FSMC_Addr_ILI9341_DATA));
}

static void ILI9341_Delay(volatile uint32_t nCount)
{
    for ( ; nCount != 0; nCount -- );
}

void ILI9341_BackLed_Control(char enable)
{
	if (enable) {
		HAL_GPIO_WritePin(ILI9341_BK_PORT, ILI9341_BK_PIN, GPIO_PIN_RESET);
    }
	else {
		HAL_GPIO_WritePin(ILI9341_BK_PORT, ILI9341_BK_PIN, GPIO_PIN_SET);
	}	
}

void ILI9341_Rst ( void )
{			
	HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_RESET);	
	ILI9341_Delay ( 0xAFFF<<2 ); 					   

	HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_SET);	
	ILI9341_Delay ( 0xAFFF<<2 ); 	
	
}

//////////////////////////////////////////////////////////////////////////////
// Public Function
//

void ili9341_Init(void)
{
    /* Initialize ILI9341 low level bus layer ----------------------------------*/
    LCD_IO_Init();

	ILI9341_BackLed_Control(0x01); 
	ILI9341_Rst();    
    
	/*  Power control B (CFh)  */
	ILI9341_Write_Cmd ( 0xCF  );
	ILI9341_Write_Data ( 0x00  );
	ILI9341_Write_Data ( 0x81  );
	ILI9341_Write_Data ( 0x30  );
	
	/*  Power on sequence control (EDh) */
	ILI9341_Write_Cmd ( 0xED );
	ILI9341_Write_Data ( 0x64 );
	ILI9341_Write_Data ( 0x03 );
	ILI9341_Write_Data ( 0x12 );
	ILI9341_Write_Data ( 0x81 );
	
	/*  Driver timing control A (E8h) */
	ILI9341_Write_Cmd ( 0xE8 ); 
	ILI9341_Write_Data ( 0x85 );
	ILI9341_Write_Data ( 0x10 );
	ILI9341_Write_Data ( 0x78 );
	
	/*  Power control A (CBh) */
	ILI9341_Write_Cmd ( 0xCB );
	ILI9341_Write_Data ( 0x39 );
	ILI9341_Write_Data ( 0x2C );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x34 );
	ILI9341_Write_Data ( 0x02 );
	
	/* Pump ratio control (F7h) */
	ILI9341_Write_Cmd ( 0xF7 );
	ILI9341_Write_Data ( 0x20 );
	
	/* Driver timing control B */
	ILI9341_Write_Cmd ( 0xEA );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x00 );
	
	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	ILI9341_Write_Cmd ( 0xB1 );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x1B );
	
	/*  Display Function Control (B6h) */
	ILI9341_Write_Cmd ( 0xB6 );
	ILI9341_Write_Data ( 0x0A );
	ILI9341_Write_Data ( 0xA2 );
	
	/* Power Control 1 (C0h) */
	ILI9341_Write_Cmd ( 0xC0 );
	ILI9341_Write_Data ( 0x35 );
	
	/* Power Control 2 (C1h) */
	ILI9341_Write_Cmd ( 0xC1 );
	ILI9341_Write_Data ( 0x11 );
	
	/* VCOM Control 1 (C5h) */
	ILI9341_Write_Cmd ( 0xC5 );
	ILI9341_Write_Data ( 0x45 );
	ILI9341_Write_Data ( 0x45 );
	
	/*  VCOM Control 2 (C7h)  */
	ILI9341_Write_Cmd ( 0xC7 );
	ILI9341_Write_Data ( 0xA2 );
	
	/* Enable 3G (F2h) */
	ILI9341_Write_Cmd ( 0xF2 );
	ILI9341_Write_Data ( 0x00 );
	
	/* Gamma Set (26h) */
	ILI9341_Write_Cmd ( 0x26 );
	ILI9341_Write_Data ( 0x01 );
	
	/* Positive Gamma Correction */
	ILI9341_Write_Cmd ( 0xE0 ); //Set Gamma
	ILI9341_Write_Data ( 0x0F );
	ILI9341_Write_Data ( 0x26 );
	ILI9341_Write_Data ( 0x24 );
	ILI9341_Write_Data ( 0x0B );
	ILI9341_Write_Data ( 0x0E );
	ILI9341_Write_Data ( 0x09 );
	ILI9341_Write_Data ( 0x54 );
	ILI9341_Write_Data ( 0xA8 );
	ILI9341_Write_Data ( 0x46 );
	ILI9341_Write_Data ( 0x0C );
	ILI9341_Write_Data ( 0x17 );
	ILI9341_Write_Data ( 0x09 );
	ILI9341_Write_Data ( 0x0F );
	ILI9341_Write_Data ( 0x07 );
	ILI9341_Write_Data ( 0x00 );
	
	/* Negative Gamma Correction (E1h) */
	ILI9341_Write_Cmd ( 0XE1 ); //Set Gamma
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x19 );
	ILI9341_Write_Data ( 0x1B );
	ILI9341_Write_Data ( 0x04 );
	ILI9341_Write_Data ( 0x10 );
	ILI9341_Write_Data ( 0x07 );
	ILI9341_Write_Data ( 0x2A );
	ILI9341_Write_Data ( 0x47 );
	ILI9341_Write_Data ( 0x39 );
	ILI9341_Write_Data ( 0x03 );
	ILI9341_Write_Data ( 0x06 );
	ILI9341_Write_Data ( 0x06 );
	ILI9341_Write_Data ( 0x30 );
	ILI9341_Write_Data ( 0x38 );
	ILI9341_Write_Data ( 0x0F );
	
	/* memory access control set */
	ILI9341_Write_Cmd ( 0x36 ); 	
	ILI9341_Write_Data ( 0xC8 );    
	
	/* column address control set */
	ILI9341_Write_Cmd ( CMD_SetCoordinateX ); 
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0xEF );
	
	/* page address control set */
	ILI9341_Write_Cmd ( CMD_SetCoordinateY ); 
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x01 );
	ILI9341_Write_Data ( 0x3F );
	
	/*  Pixel Format Set (3Ah)  */
	ILI9341_Write_Cmd ( 0x3a ); 
	ILI9341_Write_Data ( 0x55 );
	
	/* Sleep Out (11h)  */
	ILI9341_Write_Cmd ( 0x11 );	
	ILI9341_Delay ( 0xAFFf<<2 );
	
	/* Display ON (29h) */
	ILI9341_Write_Cmd ( 0x29 ); 	
}

uint16_t ili9341_ReadID(void)
{
	uint16_t id = 0;
	
	ILI9341_Write_Cmd(LCD_READ_ID4);   
	
	ILI9341_Read_Data();        // Dummy Read
	ILI9341_Read_Data();        // Dummy Read
 	
    id = ILI9341_Read_Data();   // 0x93  	
    id <<= 8;
	id |= ILI9341_Read_Data();  // 0x41 	
	
    return id;
}

void ili9341_DisplayOn(void)
{
    ILI9341_Write_Cmd(LCD_DISPLAY_ON);   
}

void ili9341_DisplayOff(void)
{
    ILI9341_Write_Cmd(LCD_DISPLAY_OFF);   
}

void ili9341_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	ILI9341_Write_Cmd(CMD_SetCoordinateX); 				
	ILI9341_Write_Data(Xpos >> 8);	 
	ILI9341_Write_Data(Xpos & 0xff);

	ILI9341_Write_Cmd(CMD_SetCoordinateY); 		 	    
	ILI9341_Write_Data(Ypos >> 8); 
	ILI9341_Write_Data(Ypos & 0xff  );    
}

void ili9341_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t Color)
{
    ili9341_SetCursor(Xpos, Ypos);
	ILI9341_Write_Cmd(CMD_SetPixel);
	ILI9341_Write_Data(Color);
}

uint16_t ili9341_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
	uint16_t usR=0, usG=0, usB=0 ;
	
	ILI9341_Write_Cmd(LCD_RAMRD);   
	
	usR = ILI9341_Read_Data (); 	/*FIRST READ OUT DUMMY DATA*/

	usR = ILI9341_Read_Data ();  	
	usB = ILI9341_Read_Data ();  	
	usG = ILI9341_Read_Data ();  	
	
    return ( ( ( usR >> 11 ) << 11 ) | ( ( usG >> 10 ) << 5 ) | ( usB >> 11 ) );    
}

void ili9341_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
	ILI9341_Write_Cmd(CMD_SetCoordinateX); 				
	ILI9341_Write_Data(Xpos >> 8);	 
	ILI9341_Write_Data(Xpos & 0xff);	 
	ILI9341_Write_Data((Xpos+Width-1) >> 8);
	ILI9341_Write_Data((Xpos+Width-1) & 0xff);

	ILI9341_Write_Cmd(CMD_SetCoordinateY); 		 	    
	ILI9341_Write_Data(Ypos >> 8); 
	ILI9341_Write_Data(Ypos & 0xff  );
	ILI9341_Write_Data((Ypos+Height-1 ) >> 8 );
	ILI9341_Write_Data((Ypos+Height-1) & 0xff ); 
}

void ili9341_DrawHLine(uint16_t Color, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
    uint16_t index = 0;
    
    for(index = 0; index < Length; index++) {
        ili9341_WritePixel((Xpos + index), Ypos, Color);
    }
}

void ili9341_DrawVLine(uint16_t Color, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
    uint16_t index = 0;
    
    for(index = 0; index < Length; index++)
    {
        ili9341_WritePixel(Xpos, (Ypos + index), Color);
    }
}

uint16_t ili9341_GetLcdPixelWidth(void)
{
    return ILI9341_LCD_PIXEL_WIDTH;
}

uint16_t ili9341_GetLcdPixelHeight(void)
{
    return ILI9341_LCD_PIXEL_HEIGHT;
}

void ili9341_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
    int i, j;
    uint32_t index = 0, size = 0;
    uint16_t width, height;
    uint16_t point;
    
    /* Read bitmap size */
    size   =  *(volatile uint16_t *) (pbmp + 2);
    size  |= (*(volatile uint16_t *) (pbmp + 4)) << 16;
    
    /* Get bitmap data address offset */
    index  =  *(volatile uint16_t *) (pbmp + 10);
    index |= (*(volatile uint16_t *) (pbmp + 12)) << 16;
    
    width  =  *(volatile uint16_t *) (pbmp + 18);
    width |= (*(volatile uint16_t *) (pbmp + 20)) << 16;

    height  =  *(volatile uint16_t *) (pbmp + 22);
    height |= (*(volatile uint16_t *) (pbmp + 24)) << 16;
    
    size   = (size - index)/2;
    pbmp  += index;
   
    for(i=height; i>0; i--) {
        for(j=0; j<width; j++) {
           point  = *(volatile uint16_t *) pbmp;
           ili9341_WritePixel(j+Xpos, i+Ypos, point);
           pbmp+=2;
        }
    } 
}

void ili9341_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp)
{
    int i, j;
    uint16_t point;
         
    for(i=Ysize; i>0; i--) {
        for(j=0; j<Xsize; j++) {
           point  = *(volatile uint16_t *) pbmp;
           ili9341_WritePixel(j+Xpos, i+Ypos, point);
           pbmp+=2;
        }
    }     
}
