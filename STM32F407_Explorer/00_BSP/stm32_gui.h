#ifndef __STM32_GUI_H__
#define __STM32_GUI_H__

#include "gui.h"

#ifdef USE_STM32412G_DISCOVERY
    #include "stm32412g_discovery_lcd.h"
#endif

#ifdef USE_STM32F723E_DISCOVERY
    #include "stm32f723e_discovery_lcd.h"
#endif

#ifdef USE_STM32F103_FIRE
    #include "stm32f103_fire_lcd.h"
#endif  

int32_t STM32_GUI_Init(void);
int32_t STM32_GUI_GetXSize(void);
int32_t STM32_GUI_GetYSize(void);
int32_t STM32_GUI_ReadPixel(uint16_t Xpos, uint16_t Ypos, uint32_t *Color);
int32_t STM32_GUI_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t Color);
int32_t STM32_GUI_Clear(uint32_t Color);
int32_t STM32_GUI_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
int32_t STM32_GUI_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp);

extern GUI_Interface_T stm32_gui_interface;
 
#endif//__GUI_STM32_H__
