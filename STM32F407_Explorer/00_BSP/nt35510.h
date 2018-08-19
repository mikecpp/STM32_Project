#ifndef __nt35510_H
#define __nt35510_H

#include "lcd.h"

#define nt35510_ID                  0x5510

#define  nt35510_LCD_PIXEL_WIDTH    ((uint16_t) 480)
#define  nt35510_LCD_PIXEL_HEIGHT   ((uint16_t) 800)

typedef struct {										    
	uint16_t    width;			
	uint16_t    height;			
	uint16_t    id;				
	uint8_t     dir;			
	uint16_t	wramcmd;		
	uint16_t    setxcmd;		
	uint16_t    setycmd;	
} _lcd_dev; 	  

#define LCD_VERT        0
#define LCD_HORI        1

#define L2R_U2D         0 
#define L2R_D2U         1 
#define R2L_U2D         2 
#define R2L_D2U         3 

#define U2D_L2R         4 
#define U2D_R2L         5 
#define D2U_L2R         6 
#define D2U_R2L         7 

void     nt35510_Init(void);
uint16_t nt35510_ReadID(void);
void     nt35510_DisplayOn(void);
void     nt35510_DisplayOff(void);
void     nt35510_Dir(uint8_t val);
void     nt35510_SetCursor(uint16_t Xpos, uint16_t Ypos);
void     nt35510_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t Color);
uint16_t nt35510_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void     nt35510_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     nt35510_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t Color);
void     nt35510_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t Color);
uint16_t nt35510_GetLcdPixelWidth(void);
uint16_t nt35510_GetLcdPixelHeight(void);
void     nt35510_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint16_t Color);
void     nt35510_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
void     nt35510_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp);

/* LCD driver structure */
extern LCD_DrvTypeDef   nt35510_drv;

/* LCD IO functions */
void     LCD_IO_Init(void);
void     LCD_IO_WriteData(uint16_t RegValue);
void     LCD_IO_WriteReg(uint8_t Reg);
uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize);
void     LCD_Delay (uint32_t delay);
      
#endif /* __nt35510_H */
