#ifndef __STM32F407_EXPLORER_LCD_H__
#define __STM32F407_EXPLORER_LCD_H__

#ifdef __cplusplus
 extern "C" {
#endif 

#if defined(USE_STM32F407_MINI)
    #include "ili9341.h" 
#else // USE_STM32F407_EXPLORER
    #include "nt35510.h"
#endif

#include "fonts.h" 

typedef struct 
{ 
  uint32_t TextColor;
  uint32_t BackColor;
  sFONT    *pFont; 

}LCD_DrawPropTypeDef;

#define LCD_OK         0x00
#define LCD_ERROR      0x01
#define LCD_TIMEOUT    0x02
    
typedef struct 
{
  int16_t X;
  int16_t Y;

}Point, * pPoint; 

typedef enum
{
  CENTER_MODE             = 0x01,    /*!< Center mode */
  RIGHT_MODE              = 0x02,    /*!< Right mode  */
  LEFT_MODE               = 0x03     /*!< Left mode   */

}Line_ModeTypdef;

#define LCD_COLOR_BLUE          0x001F
#define LCD_COLOR_GREEN         0x07E0
#define LCD_COLOR_RED           0xF800
#define LCD_COLOR_CYAN          0x07FF
#define LCD_COLOR_MAGENTA       0xF81F
#define LCD_COLOR_YELLOW        0xFFE0
#define LCD_COLOR_LIGHTBLUE     0x841F
#define LCD_COLOR_LIGHTGREEN    0x87F0
#define LCD_COLOR_LIGHTRED      0xFC10
#define LCD_COLOR_LIGHTCYAN     0x87FF
#define LCD_COLOR_LIGHTMAGENTA  0xFC1F
#define LCD_COLOR_LIGHTYELLOW   0xFFF0
#define LCD_COLOR_DARKBLUE      0x0010
#define LCD_COLOR_DARKGREEN     0x0400
#define LCD_COLOR_DARKRED       0x8000
#define LCD_COLOR_DARKCYAN      0x0410
#define LCD_COLOR_DARKMAGENTA   0x8010
#define LCD_COLOR_DARKYELLOW    0x8400
#define LCD_COLOR_WHITE         0xFFFF
#define LCD_COLOR_LIGHTGRAY     0xD69A
#define LCD_COLOR_GRAY          0x8410
#define LCD_COLOR_DARKGRAY      0x4208
#define LCD_COLOR_BLACK         0x0000
#define LCD_COLOR_BROWN         0xA145
#define LCD_COLOR_ORANGE        0xFD20

#define LCD_DEFAULT_FONT        Font16

uint8_t  LCD_Init(void);
uint32_t LCD_GetXSize(void);
uint32_t LCD_GetYSize(void);
 
uint16_t LCD_GetTextColor(void);
uint16_t LCD_GetBackColor(void);
void     LCD_SetTextColor(volatile uint16_t Color);
void     LCD_SetBackColor(volatile uint16_t Color);
void     LCD_SetFont(sFONT *fonts);
sFONT    *LCD_GetFont(void);

void     LCD_Clear(uint16_t Color);
void     LCD_ClearStringLine(uint16_t Line);
void     LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr);
void     LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Line_ModeTypdef Mode);
void     LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);

void     LCD_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t Color);
uint16_t LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void     LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void     LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     LCD_DrawPolygon(pPoint Points, uint16_t PointCount);
void     LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void     LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
void     LCD_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp);
void     LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);

void     LCD_DisplayOff(void);
void     LCD_DisplayOn(void);
 
#ifdef __cplusplus
}
#endif

#endif /* __STM32F407_EXPLORER_LCD_H__ */
