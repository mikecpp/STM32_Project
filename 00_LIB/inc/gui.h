#ifndef __GUI_H__
#define __GUI_H__

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
#include <fonts.h>

typedef struct {
    int32_t (*GUI_Init)(void);
    int32_t (*GUI_GetXSize)(void);
    int32_t (*GUI_GetYSize)(void);
    int32_t (*GUI_ReadPixel)(uint16_t Xpos, uint16_t Ypos, uint32_t *Color);
    int32_t (*GUI_DrawPixel)(uint16_t XPos, uint16_t YPos, uint32_t Color);
    int32_t (*GUI_Clear)(uint32_t Color);
    int32_t (*GUI_DrawBitmap)(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
    int32_t (*GUI_DrawRGBImage)(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp);
} GUI_Interface_T;

#define GUI_COLOR_BLACK     0x00000000
#define GUI_COLOR_WHITE     0xFFFFFFFF
#define GUI_COLOR_RED       0x00FF0000
#define GUI_COLOR_GREEN     0x0000FF00
#define GUI_COLOR_BLUE      0x000000FF
#define GUI_COLOR_YELLOW    0x00FFFF00

typedef enum {
   GUI_CENTER_MODE          = 0x01,    
   GUI_RIGHT_MODE           = 0x02,    
   GUI_LEFT_MODE            = 0x03     
} GUI_Line_Mode_T;

void GUI_Register(GUI_Interface_T *interface);

// Platform Dependency 
int32_t GUI_Init(void); 
int32_t GUI_GetXSize(void); 
int32_t GUI_GetYSize(void); 
int32_t GUI_ReadPixel(uint16_t Xpos, uint16_t Ypos, uint32_t *Color);
int32_t GUI_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t Color);
int32_t GUI_Clear(uint32_t Color);
int32_t GUI_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
int32_t GUI_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp);

// Platform Independency
void GUI_SetFont(sFONT *fonts);
sFONT* GUI_GetFont(void);

int32_t GUI_GetTextColor(uint32_t *Color);
int32_t GUI_GetBackColor(uint32_t *Color);
int32_t GUI_SetTextColor(uint32_t Color);
int32_t GUI_SetBackColor(uint32_t Color);

void GUI_ClearStringLine(uint16_t Line);
void GUI_DisplayStringAtLine(uint16_t Line, uint8_t *ptr);
void GUI_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, GUI_Line_Mode_T Mode);
void GUI_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);

void GUI_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void GUI_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void GUI_DrawLine(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2);
void GUI_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void GUI_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void GUI_DrawEllipse(uint16_t Xpos, uint16_t Ypos, uint16_t XRadius, uint16_t YRadius);
void GUI_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void GUI_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void GUI_FillEllipse(uint16_t Xpos, uint16_t Ypos, uint16_t XRadius, uint16_t YRadius);

#ifdef __cplusplus
}
#endif

#endif//__GUI_H__
