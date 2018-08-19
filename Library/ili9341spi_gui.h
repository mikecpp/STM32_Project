#ifndef __ILI9341_GUI_H__
#define __ILI9341_GUI_H__

#include "gui.h"

int32_t ILI9341_GUI_Init(void);
int32_t ILI9341_GUI_GetXSize(void);
int32_t ILI9341_GUI_GetYSize(void);
int32_t ILI9341_GUI_ReadPixel(uint16_t Xpos, uint16_t Ypos, uint32_t *Color);
int32_t ILI9341_GUI_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t Color);
int32_t ILI9341_GUI_Clear(uint32_t Color);
int32_t ILI9341_GUI_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
int32_t ILI9341_GUI_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp);

extern GUI_Interface_T ili9341_gui_interface;
 
#endif//__ILI9341_GUI_H__
