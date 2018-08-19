#include "nt35510.h"
#include "stm32f4xx.h"
#include "stm32f407_explorer.h"
#include "stm32f407_system.h"

LCD_DrvTypeDef nt35510_drv = 
{
    nt35510_Init,
    nt35510_ReadID,
    nt35510_DisplayOn,
    nt35510_DisplayOff,
    nt35510_SetCursor,
    nt35510_WritePixel,
    nt35510_ReadPixel,
    nt35510_SetDisplayWindow,
    nt35510_DrawHLine,
    nt35510_DrawVLine,
    nt35510_GetLcdPixelWidth,
    nt35510_GetLcdPixelHeight,
    nt35510_FillRect,
    nt35510_DrawBitmap,
    nt35510_DrawRGBImage
};

typedef struct
{
	uint16_t LCD_REG;
	uint16_t LCD_RAM;
} LCD_TypeDef;
		    
#define LCD_BASE        ((uint32_t) (0x6C000000 | 0x0000007E)) // Bank4
#define LCD             ((LCD_TypeDef *) LCD_BASE)

// Reset PIN: PE1
#define      nt35510_RST_PORT               GPIOE               // RST: PE1
#define      nt35510_RST_PIN                GPIO_PIN_1

#define      nt35510_BK_PORT                GPIOB               // BK_LIGHT: PB15
#define      nt35510_BK_PIN                 GPIO_PIN_15

_lcd_dev m_lcddev;

void LCD_WR_REG(__IO uint16_t regval)
{   
	regval = regval;		
	LCD->LCD_REG = regval;	 
}

void LCD_WR_DATA(__IO uint16_t data)
{	  
	data = data;			
	LCD->LCD_RAM = data;		 
}

uint16_t LCD_RD_DATA(void)
{
	uint16_t ram;	
    
	ram = LCD->LCD_RAM;	

	return ram;	 
}					   

void LCD_WriteReg(__IO uint16_t LCD_Reg, __IO uint16_t LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		 
	LCD->LCD_RAM = LCD_RegValue;	    		 
}	   

uint16_t LCD_ReadReg(__IO uint16_t LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		
    
	return LCD_RD_DATA();		
}   

void LCD_Scan_Dir(uint8_t dir)
{
	uint16_t regval = 0;
	uint16_t dirreg = 0;
	// uint16_t temp = 0;
    
    switch(dir)
    {
        case L2R_U2D:
            regval |= (0<<7)|(0<<6)|(0<<5); 
            break;
        case L2R_D2U:
            regval |= (1<<7)|(0<<6)|(0<<5); 
            break;
        case R2L_U2D:
            regval |= (0<<7)|(1<<6)|(0<<5); 
            break;
        case R2L_D2U:
            regval |= (1<<7)|(1<<6)|(0<<5); 
            break;	 
        case U2D_L2R:
            regval |= (0<<7)|(0<<6)|(1<<5); 
            break;
        case U2D_R2L:
            regval |= (0<<7)|(1<<6)|(1<<5); 
            break;
        case D2U_L2R:
            regval |= (1<<7)|(0<<6)|(1<<5); 
            break;
        case D2U_R2L:
            regval |= (1<<7)|(1<<6)|(1<<5); 
            break;	 
    }
		
    dirreg = 0X3600;
    LCD_WriteReg(dirreg, regval);

    LCD_WR_REG(m_lcddev.setxcmd);
    LCD_WR_DATA(0); 
    
    LCD_WR_REG(m_lcddev.setxcmd+1);
    LCD_WR_DATA(0); 
    
    LCD_WR_REG(m_lcddev.setxcmd+2);
    LCD_WR_DATA((m_lcddev.width-1) >> 8); 
    
    LCD_WR_REG(m_lcddev.setxcmd+3);
    LCD_WR_DATA((m_lcddev.width-1) & 0XFF); 
    
    LCD_WR_REG(m_lcddev.setycmd);
    LCD_WR_DATA(0); 
    
    LCD_WR_REG(m_lcddev.setycmd+1);
    LCD_WR_DATA(0); 
    
    LCD_WR_REG(m_lcddev.setycmd+2);
    LCD_WR_DATA((m_lcddev.height-1) >> 8); 
    
    LCD_WR_REG(m_lcddev.setycmd+3);
    LCD_WR_DATA((m_lcddev.height-1) & 0XFF);
}     

//////////////////////////////////////////////////////////////////////////////
// Public Function
//

uint16_t nt35510_ReadID(void)
{
    return m_lcddev.id;
}

void nt35510_DisplayOn(void)
{
    LCD_WR_REG(0X2900); 
}

void nt35510_DisplayOff(void)
{
    LCD_WR_REG(0X2800);  
}

void nt35510_Dir(uint8_t val)
{
    m_lcddev.dir = val;
    
    if(val == LCD_VERT) {
        m_lcddev.wramcmd = 0X2C00;
	 	m_lcddev.setxcmd = 0X2A00;
		m_lcddev.setycmd = 0X2B00; 
		m_lcddev.width   = 480;
		m_lcddev.height  = 800;       
        LCD_Scan_Dir(L2R_U2D);        
    }
    else { // LCD_HORI
		m_lcddev.wramcmd = 0X2C00;
 		m_lcddev.setxcmd = 0X2B00;
		m_lcddev.setycmd = 0X2A00; 
		m_lcddev.width   = 800;
		m_lcddev.height  = 480;        
        LCD_Scan_Dir(L2R_D2U);
    }
}

void nt35510_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    LCD_WR_REG(m_lcddev.setxcmd);
    LCD_WR_DATA(Xpos >> 8); 		
    
    LCD_WR_REG(m_lcddev.setxcmd + 1);
    LCD_WR_DATA(Xpos & 0XFF);			 
    
    LCD_WR_REG(m_lcddev.setycmd);
    LCD_WR_DATA(Ypos >> 8);  		
    
    LCD_WR_REG(m_lcddev.setycmd + 1);
    LCD_WR_DATA(Ypos & 0XFF);	
}

void nt35510_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t Color)
{
    nt35510_SetCursor(Xpos, Ypos);
    
	LCD->LCD_REG = m_lcddev.wramcmd; 
	LCD->LCD_RAM = Color;      
}

uint16_t nt35510_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
	uint16_t r=0, g=0, b=0 ;
	
	LCD_WR_REG(0X2E00);
	
    r = LCD_RD_DATA(); // Read R + G
	b = LCD_RD_DATA(); // Read B	
    g = r & 0xFF;
    g <<= 8;
    
    return ( ( ( r >> 11 ) << 11 ) | ( ( g >> 10 ) << 5 ) | ( b >> 11 ) );    
}

void nt35510_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    uint16_t Xend = Xpos + Width  - 1;
    uint16_t Yend = Ypos + Height - 1;
    
    LCD_WR_REG(m_lcddev.setxcmd);
    LCD_WR_DATA(Xpos >> 8);
    
    LCD_WR_REG(m_lcddev.setxcmd+1);
    LCD_WR_DATA(Xpos & 0XFF);	  
   
    LCD_WR_REG(m_lcddev.setxcmd+2);
    LCD_WR_DATA(Xend >> 8);   

    LCD_WR_REG(m_lcddev.setxcmd+3);
    LCD_WR_DATA(Xend & 0XFF);   

    LCD_WR_REG(m_lcddev.setycmd);
    LCD_WR_DATA(Ypos >> 8);   

    LCD_WR_REG(m_lcddev.setycmd+1);
    LCD_WR_DATA(Ypos & 0XFF);  

    LCD_WR_REG(m_lcddev.setycmd+2);
    LCD_WR_DATA(Yend >> 8);   

    LCD_WR_REG(m_lcddev.setycmd+3);
    LCD_WR_DATA(Yend & 0XFF);  
}

void nt35510_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t Color)
{
	nt35510_FillRect(Xpos, Ypos, Length, 1, Color);
}

void nt35510_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t Color)
{
	nt35510_FillRect(Xpos, Ypos, 1, Length, Color);
}

uint16_t nt35510_GetLcdPixelWidth(void)
{
    return m_lcddev.width;
}

uint16_t nt35510_GetLcdPixelHeight(void)
{
    return m_lcddev.height;
}

void nt35510_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint16_t Color)
{
    uint16_t i, j;
    
    nt35510_SetDisplayWindow(Xpos, Ypos, Xsize, Ysize);
    LCD->LCD_REG = 0x2C00;             

    for(i=0; i<Xsize; i++) {
       for(j=0; j<Ysize; j++) {
           LCD->LCD_RAM = Color;
       }    
   } 
}

void nt35510_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
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
           nt35510_WritePixel(j+Xpos, i+Ypos, point);
           pbmp+=2;
        }
    } 
}

void nt35510_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp)
{
    int i, j;
    uint16_t point;
         
    for(i=Ysize; i>0; i--) {
        for(j=0; j<Xsize; j++) {
           point  = *(volatile uint16_t *) pbmp;
           nt35510_WritePixel(j+Xpos, i+Ypos, point);
           pbmp+=2;
        }
    }     
}

void nt35510_Init(void)
{ 
    BSP_FSMC_Init();
    
    delay_ms(50);
    LCD_WriteReg(0x0000, 0x0001);
    delay_ms(50);
    m_lcddev.id = LCD_ReadReg(0x0000);   

    // Read NT35510 ID 
    LCD_WR_REG(0xDA00);	
	m_lcddev.id  = LCD_RD_DATA();
    
    LCD_WR_REG(0xDB00);	
    m_lcddev.id  = LCD_RD_DATA();	
    m_lcddev.id <<= 8;	
    
    LCD_WR_REG(0xDC00);	
	m_lcddev.id |= LCD_RD_DATA();			
	
    if(m_lcddev.id == 0x8000) {
        m_lcddev.id = 0x5510;
    }
    
    printf("LCD ID = %x \r\n", m_lcddev.id);

    FSMC_Bank1E->BWTR[6] &= ~(0XF<<0);	 // ADDSET 0
	FSMC_Bank1E->BWTR[6] &= ~(0XF<<8);   
	FSMC_Bank1E->BWTR[6] |= 3<<0;		 // 18ns	 
	FSMC_Bank1E->BWTR[6] |= 2<<8; 	     // 18ns

    LCD_WriteReg(0xF000,0x55);
    LCD_WriteReg(0xF001,0xAA);
    LCD_WriteReg(0xF002,0x52);
    LCD_WriteReg(0xF003,0x08);
    LCD_WriteReg(0xF004,0x01);
    
    //AVDD Set AVDD 5.2V
    LCD_WriteReg(0xB000,0x0D);
    LCD_WriteReg(0xB001,0x0D);
    LCD_WriteReg(0xB002,0x0D);
    
    //AVDD ratio
    LCD_WriteReg(0xB600,0x34);
    LCD_WriteReg(0xB601,0x34);
    LCD_WriteReg(0xB602,0x34);
    
    //AVEE -5.2V
    LCD_WriteReg(0xB100,0x0D);
    LCD_WriteReg(0xB101,0x0D);
    LCD_WriteReg(0xB102,0x0D);
    
    //AVEE ratio
    LCD_WriteReg(0xB700,0x34);
    LCD_WriteReg(0xB701,0x34);
    LCD_WriteReg(0xB702,0x34);
    
    //VCL -2.5V
    LCD_WriteReg(0xB200,0x00);
    LCD_WriteReg(0xB201,0x00);
    LCD_WriteReg(0xB202,0x00);
    
    //VCL ratio
    LCD_WriteReg(0xB800,0x24);
    LCD_WriteReg(0xB801,0x24);
    LCD_WriteReg(0xB802,0x24);
    
    //VGH 15V (Free pump)
    LCD_WriteReg(0xBF00,0x01);
    LCD_WriteReg(0xB300,0x0F);
    LCD_WriteReg(0xB301,0x0F);
    LCD_WriteReg(0xB302,0x0F);
    
    //VGH ratio
    LCD_WriteReg(0xB900,0x34);
    LCD_WriteReg(0xB901,0x34);
    LCD_WriteReg(0xB902,0x34);
    
    //VGL_REG -10V
    LCD_WriteReg(0xB500,0x08);
    LCD_WriteReg(0xB501,0x08);
    LCD_WriteReg(0xB502,0x08);
    LCD_WriteReg(0xC200,0x03);
    
    //VGLX ratio
    LCD_WriteReg(0xBA00,0x24);
    LCD_WriteReg(0xBA01,0x24);
    LCD_WriteReg(0xBA02,0x24);
    
    //VGMP/VGSP 4.5V/0V
    LCD_WriteReg(0xBC00,0x00);
    LCD_WriteReg(0xBC01,0x78);
    LCD_WriteReg(0xBC02,0x00);
    
    //VGMN/VGSN -4.5V/0V
    LCD_WriteReg(0xBD00,0x00);
    LCD_WriteReg(0xBD01,0x78);
    LCD_WriteReg(0xBD02,0x00);
    
    //VCOM
    LCD_WriteReg(0xBE00,0x00);
    LCD_WriteReg(0xBE01,0x64);
    
    //Gamma Setting
    LCD_WriteReg(0xD100,0x00);
    LCD_WriteReg(0xD101,0x33);
    LCD_WriteReg(0xD102,0x00);
    LCD_WriteReg(0xD103,0x34);
    LCD_WriteReg(0xD104,0x00);
    LCD_WriteReg(0xD105,0x3A);
    LCD_WriteReg(0xD106,0x00);
    LCD_WriteReg(0xD107,0x4A);
    LCD_WriteReg(0xD108,0x00);
    LCD_WriteReg(0xD109,0x5C);
    LCD_WriteReg(0xD10A,0x00);
    LCD_WriteReg(0xD10B,0x81);
    LCD_WriteReg(0xD10C,0x00);
    LCD_WriteReg(0xD10D,0xA6);
    LCD_WriteReg(0xD10E,0x00);
    LCD_WriteReg(0xD10F,0xE5);
    LCD_WriteReg(0xD110,0x01);
    LCD_WriteReg(0xD111,0x13);
    LCD_WriteReg(0xD112,0x01);
    LCD_WriteReg(0xD113,0x54);
    LCD_WriteReg(0xD114,0x01);
    LCD_WriteReg(0xD115,0x82);
    LCD_WriteReg(0xD116,0x01);
    LCD_WriteReg(0xD117,0xCA);
    LCD_WriteReg(0xD118,0x02);
    LCD_WriteReg(0xD119,0x00);
    LCD_WriteReg(0xD11A,0x02);
    LCD_WriteReg(0xD11B,0x01);
    LCD_WriteReg(0xD11C,0x02);
    LCD_WriteReg(0xD11D,0x34);
    LCD_WriteReg(0xD11E,0x02);
    LCD_WriteReg(0xD11F,0x67);
    LCD_WriteReg(0xD120,0x02);
    LCD_WriteReg(0xD121,0x84);
    LCD_WriteReg(0xD122,0x02);
    LCD_WriteReg(0xD123,0xA4);
    LCD_WriteReg(0xD124,0x02);
    LCD_WriteReg(0xD125,0xB7);
    LCD_WriteReg(0xD126,0x02);
    LCD_WriteReg(0xD127,0xCF);
    LCD_WriteReg(0xD128,0x02);
    LCD_WriteReg(0xD129,0xDE);
    LCD_WriteReg(0xD12A,0x02);
    LCD_WriteReg(0xD12B,0xF2);
    LCD_WriteReg(0xD12C,0x02);
    LCD_WriteReg(0xD12D,0xFE);
    LCD_WriteReg(0xD12E,0x03);
    LCD_WriteReg(0xD12F,0x10);
    LCD_WriteReg(0xD130,0x03);
    LCD_WriteReg(0xD131,0x33);
    LCD_WriteReg(0xD132,0x03);
    LCD_WriteReg(0xD133,0x6D);
    LCD_WriteReg(0xD200,0x00);
    LCD_WriteReg(0xD201,0x33);
    LCD_WriteReg(0xD202,0x00);
    LCD_WriteReg(0xD203,0x34);
    LCD_WriteReg(0xD204,0x00);
    LCD_WriteReg(0xD205,0x3A);
    LCD_WriteReg(0xD206,0x00);
    LCD_WriteReg(0xD207,0x4A);
    LCD_WriteReg(0xD208,0x00);
    LCD_WriteReg(0xD209,0x5C);
    LCD_WriteReg(0xD20A,0x00);

    LCD_WriteReg(0xD20B,0x81);
    LCD_WriteReg(0xD20C,0x00);
    LCD_WriteReg(0xD20D,0xA6);
    LCD_WriteReg(0xD20E,0x00);
    LCD_WriteReg(0xD20F,0xE5);
    LCD_WriteReg(0xD210,0x01);
    LCD_WriteReg(0xD211,0x13);
    LCD_WriteReg(0xD212,0x01);
    LCD_WriteReg(0xD213,0x54);
    LCD_WriteReg(0xD214,0x01);
    LCD_WriteReg(0xD215,0x82);
    LCD_WriteReg(0xD216,0x01);
    LCD_WriteReg(0xD217,0xCA);
    LCD_WriteReg(0xD218,0x02);
    LCD_WriteReg(0xD219,0x00);
    LCD_WriteReg(0xD21A,0x02);
    LCD_WriteReg(0xD21B,0x01);
    LCD_WriteReg(0xD21C,0x02);
    LCD_WriteReg(0xD21D,0x34);
    LCD_WriteReg(0xD21E,0x02);
    LCD_WriteReg(0xD21F,0x67);
    LCD_WriteReg(0xD220,0x02);
    LCD_WriteReg(0xD221,0x84);
    LCD_WriteReg(0xD222,0x02);
    LCD_WriteReg(0xD223,0xA4);
    LCD_WriteReg(0xD224,0x02);
    LCD_WriteReg(0xD225,0xB7);
    LCD_WriteReg(0xD226,0x02);
    LCD_WriteReg(0xD227,0xCF);
    LCD_WriteReg(0xD228,0x02);
    LCD_WriteReg(0xD229,0xDE);
    LCD_WriteReg(0xD22A,0x02);
    LCD_WriteReg(0xD22B,0xF2);
    LCD_WriteReg(0xD22C,0x02);
    LCD_WriteReg(0xD22D,0xFE);
    LCD_WriteReg(0xD22E,0x03);
    LCD_WriteReg(0xD22F,0x10);
    LCD_WriteReg(0xD230,0x03);
    LCD_WriteReg(0xD231,0x33);
    LCD_WriteReg(0xD232,0x03);
    LCD_WriteReg(0xD233,0x6D);
    LCD_WriteReg(0xD300,0x00);
    LCD_WriteReg(0xD301,0x33);
    LCD_WriteReg(0xD302,0x00);
    LCD_WriteReg(0xD303,0x34);
    LCD_WriteReg(0xD304,0x00);
    LCD_WriteReg(0xD305,0x3A);
    LCD_WriteReg(0xD306,0x00);
    LCD_WriteReg(0xD307,0x4A);
    LCD_WriteReg(0xD308,0x00);
    LCD_WriteReg(0xD309,0x5C);
    LCD_WriteReg(0xD30A,0x00);

    LCD_WriteReg(0xD30B,0x81);
    LCD_WriteReg(0xD30C,0x00);
    LCD_WriteReg(0xD30D,0xA6);
    LCD_WriteReg(0xD30E,0x00);
    LCD_WriteReg(0xD30F,0xE5);
    LCD_WriteReg(0xD310,0x01);
    LCD_WriteReg(0xD311,0x13);
    LCD_WriteReg(0xD312,0x01);
    LCD_WriteReg(0xD313,0x54);
    LCD_WriteReg(0xD314,0x01);
    LCD_WriteReg(0xD315,0x82);
    LCD_WriteReg(0xD316,0x01);
    LCD_WriteReg(0xD317,0xCA);
    LCD_WriteReg(0xD318,0x02);
    LCD_WriteReg(0xD319,0x00);
    LCD_WriteReg(0xD31A,0x02);
    LCD_WriteReg(0xD31B,0x01);
    LCD_WriteReg(0xD31C,0x02);
    LCD_WriteReg(0xD31D,0x34);
    LCD_WriteReg(0xD31E,0x02);
    LCD_WriteReg(0xD31F,0x67);
    LCD_WriteReg(0xD320,0x02);
    LCD_WriteReg(0xD321,0x84);
    LCD_WriteReg(0xD322,0x02);
    LCD_WriteReg(0xD323,0xA4);
    LCD_WriteReg(0xD324,0x02);
    LCD_WriteReg(0xD325,0xB7);
    LCD_WriteReg(0xD326,0x02);
    LCD_WriteReg(0xD327,0xCF);
    LCD_WriteReg(0xD328,0x02);
    LCD_WriteReg(0xD329,0xDE);
    LCD_WriteReg(0xD32A,0x02);
    LCD_WriteReg(0xD32B,0xF2);
    LCD_WriteReg(0xD32C,0x02);
    LCD_WriteReg(0xD32D,0xFE);
    LCD_WriteReg(0xD32E,0x03);
    LCD_WriteReg(0xD32F,0x10);
    LCD_WriteReg(0xD330,0x03);
    LCD_WriteReg(0xD331,0x33);
    LCD_WriteReg(0xD332,0x03);
    LCD_WriteReg(0xD333,0x6D);
    LCD_WriteReg(0xD400,0x00);
    LCD_WriteReg(0xD401,0x33);
    LCD_WriteReg(0xD402,0x00);
    LCD_WriteReg(0xD403,0x34);
    LCD_WriteReg(0xD404,0x00);
    LCD_WriteReg(0xD405,0x3A);
    LCD_WriteReg(0xD406,0x00);
    LCD_WriteReg(0xD407,0x4A);
    LCD_WriteReg(0xD408,0x00);
    LCD_WriteReg(0xD409,0x5C);
    LCD_WriteReg(0xD40A,0x00);
    LCD_WriteReg(0xD40B,0x81);

    LCD_WriteReg(0xD40C,0x00);
    LCD_WriteReg(0xD40D,0xA6);
    LCD_WriteReg(0xD40E,0x00);
    LCD_WriteReg(0xD40F,0xE5);
    LCD_WriteReg(0xD410,0x01);
    LCD_WriteReg(0xD411,0x13);
    LCD_WriteReg(0xD412,0x01);
    LCD_WriteReg(0xD413,0x54);
    LCD_WriteReg(0xD414,0x01);
    LCD_WriteReg(0xD415,0x82);
    LCD_WriteReg(0xD416,0x01);
    LCD_WriteReg(0xD417,0xCA);
    LCD_WriteReg(0xD418,0x02);
    LCD_WriteReg(0xD419,0x00);
    LCD_WriteReg(0xD41A,0x02);
    LCD_WriteReg(0xD41B,0x01);
    LCD_WriteReg(0xD41C,0x02);
    LCD_WriteReg(0xD41D,0x34);
    LCD_WriteReg(0xD41E,0x02);
    LCD_WriteReg(0xD41F,0x67);
    LCD_WriteReg(0xD420,0x02);
    LCD_WriteReg(0xD421,0x84);
    LCD_WriteReg(0xD422,0x02);
    LCD_WriteReg(0xD423,0xA4);
    LCD_WriteReg(0xD424,0x02);
    LCD_WriteReg(0xD425,0xB7);
    LCD_WriteReg(0xD426,0x02);
    LCD_WriteReg(0xD427,0xCF);
    LCD_WriteReg(0xD428,0x02);
    LCD_WriteReg(0xD429,0xDE);
    LCD_WriteReg(0xD42A,0x02);
    LCD_WriteReg(0xD42B,0xF2);
    LCD_WriteReg(0xD42C,0x02);
    LCD_WriteReg(0xD42D,0xFE);
    LCD_WriteReg(0xD42E,0x03);
    LCD_WriteReg(0xD42F,0x10);
    LCD_WriteReg(0xD430,0x03);
    LCD_WriteReg(0xD431,0x33);
    LCD_WriteReg(0xD432,0x03);
    LCD_WriteReg(0xD433,0x6D);
    LCD_WriteReg(0xD500,0x00);
    LCD_WriteReg(0xD501,0x33);
    LCD_WriteReg(0xD502,0x00);
    LCD_WriteReg(0xD503,0x34);
    LCD_WriteReg(0xD504,0x00);
    LCD_WriteReg(0xD505,0x3A);
    LCD_WriteReg(0xD506,0x00);
    LCD_WriteReg(0xD507,0x4A);
    LCD_WriteReg(0xD508,0x00);
    LCD_WriteReg(0xD509,0x5C);
    LCD_WriteReg(0xD50A,0x00);
    LCD_WriteReg(0xD50B,0x81);

    LCD_WriteReg(0xD50C,0x00);
    LCD_WriteReg(0xD50D,0xA6);
    LCD_WriteReg(0xD50E,0x00);
    LCD_WriteReg(0xD50F,0xE5);
    LCD_WriteReg(0xD510,0x01);
    LCD_WriteReg(0xD511,0x13);
    LCD_WriteReg(0xD512,0x01);
    LCD_WriteReg(0xD513,0x54);
    LCD_WriteReg(0xD514,0x01);
    LCD_WriteReg(0xD515,0x82);
    LCD_WriteReg(0xD516,0x01);
    LCD_WriteReg(0xD517,0xCA);
    LCD_WriteReg(0xD518,0x02);
    LCD_WriteReg(0xD519,0x00);
    LCD_WriteReg(0xD51A,0x02);
    LCD_WriteReg(0xD51B,0x01);
    LCD_WriteReg(0xD51C,0x02);
    LCD_WriteReg(0xD51D,0x34);
    LCD_WriteReg(0xD51E,0x02);
    LCD_WriteReg(0xD51F,0x67);
    LCD_WriteReg(0xD520,0x02);
    LCD_WriteReg(0xD521,0x84);
    LCD_WriteReg(0xD522,0x02);
    LCD_WriteReg(0xD523,0xA4);
    LCD_WriteReg(0xD524,0x02);
    LCD_WriteReg(0xD525,0xB7);
    LCD_WriteReg(0xD526,0x02);
    LCD_WriteReg(0xD527,0xCF);
    LCD_WriteReg(0xD528,0x02);
    LCD_WriteReg(0xD529,0xDE);
    LCD_WriteReg(0xD52A,0x02);
    LCD_WriteReg(0xD52B,0xF2);
    LCD_WriteReg(0xD52C,0x02);
    LCD_WriteReg(0xD52D,0xFE);
    LCD_WriteReg(0xD52E,0x03);
    LCD_WriteReg(0xD52F,0x10);
    LCD_WriteReg(0xD530,0x03);
    LCD_WriteReg(0xD531,0x33);
    LCD_WriteReg(0xD532,0x03);
    LCD_WriteReg(0xD533,0x6D);
    LCD_WriteReg(0xD600,0x00);
    LCD_WriteReg(0xD601,0x33);
    LCD_WriteReg(0xD602,0x00);
    LCD_WriteReg(0xD603,0x34);
    LCD_WriteReg(0xD604,0x00);
    LCD_WriteReg(0xD605,0x3A);
    LCD_WriteReg(0xD606,0x00);
    LCD_WriteReg(0xD607,0x4A);
    LCD_WriteReg(0xD608,0x00);
    LCD_WriteReg(0xD609,0x5C);
    LCD_WriteReg(0xD60A,0x00);
    LCD_WriteReg(0xD60B,0x81);

    LCD_WriteReg(0xD60C,0x00);
    LCD_WriteReg(0xD60D,0xA6);
    LCD_WriteReg(0xD60E,0x00);
    LCD_WriteReg(0xD60F,0xE5);
    LCD_WriteReg(0xD610,0x01);
    LCD_WriteReg(0xD611,0x13);
    LCD_WriteReg(0xD612,0x01);
    LCD_WriteReg(0xD613,0x54);
    LCD_WriteReg(0xD614,0x01);
    LCD_WriteReg(0xD615,0x82);
    LCD_WriteReg(0xD616,0x01);
    LCD_WriteReg(0xD617,0xCA);
    LCD_WriteReg(0xD618,0x02);
    LCD_WriteReg(0xD619,0x00);
    LCD_WriteReg(0xD61A,0x02);
    LCD_WriteReg(0xD61B,0x01);
    LCD_WriteReg(0xD61C,0x02);
    LCD_WriteReg(0xD61D,0x34);
    LCD_WriteReg(0xD61E,0x02);
    LCD_WriteReg(0xD61F,0x67);
    LCD_WriteReg(0xD620,0x02);
    LCD_WriteReg(0xD621,0x84);
    LCD_WriteReg(0xD622,0x02);
    LCD_WriteReg(0xD623,0xA4);
    LCD_WriteReg(0xD624,0x02);
    LCD_WriteReg(0xD625,0xB7);
    LCD_WriteReg(0xD626,0x02);
    LCD_WriteReg(0xD627,0xCF);
    LCD_WriteReg(0xD628,0x02);
    LCD_WriteReg(0xD629,0xDE);
    LCD_WriteReg(0xD62A,0x02);
    LCD_WriteReg(0xD62B,0xF2);
    LCD_WriteReg(0xD62C,0x02);
    LCD_WriteReg(0xD62D,0xFE);
    LCD_WriteReg(0xD62E,0x03);
    LCD_WriteReg(0xD62F,0x10);
    LCD_WriteReg(0xD630,0x03);
    LCD_WriteReg(0xD631,0x33);
    LCD_WriteReg(0xD632,0x03);
    LCD_WriteReg(0xD633,0x6D);
    
    //LV2 Page 0 enable
    LCD_WriteReg(0xF000,0x55);
    LCD_WriteReg(0xF001,0xAA);
    LCD_WriteReg(0xF002,0x52);
    LCD_WriteReg(0xF003,0x08);
    LCD_WriteReg(0xF004,0x00);
    
    //Display control
    LCD_WriteReg(0xB100, 0xCC);
    LCD_WriteReg(0xB101, 0x00);
    
    //Source hold time
    LCD_WriteReg(0xB600,0x05);
    
    //Gate EQ control
    LCD_WriteReg(0xB700,0x70);
    LCD_WriteReg(0xB701,0x70);
    
    //Source EQ control (Mode 2)
    LCD_WriteReg(0xB800,0x01);
    LCD_WriteReg(0xB801,0x03);
    LCD_WriteReg(0xB802,0x03);
    LCD_WriteReg(0xB803,0x03);
    
    //Inversion mode (2-dot)
    LCD_WriteReg(0xBC00,0x02);
    LCD_WriteReg(0xBC01,0x00);
    LCD_WriteReg(0xBC02,0x00);
    
    //Timing control 4H w/ 4-delay
    LCD_WriteReg(0xC900,0xD0);
    LCD_WriteReg(0xC901,0x02);
    LCD_WriteReg(0xC902,0x50);
    LCD_WriteReg(0xC903,0x50);
    LCD_WriteReg(0xC904,0x50);
    LCD_WriteReg(0x3500,0x00);
    LCD_WriteReg(0x3A00,0x55);  //16-bit/pixel
    LCD_WR_REG(0x1100);
    
    delay_ms(1);
    LCD_WR_REG(0x2900);
    
    nt35510_Dir(LCD_HORI); 
    HAL_GPIO_WritePin(nt35510_BK_PORT, nt35510_BK_PIN, GPIO_PIN_SET); // PB15
}
