#include "stm32_gui.h"

GUI_Interface_T stm32_gui_interface = 
{
    .GUI_Init           = STM32_GUI_Init, 
    .GUI_GetXSize       = STM32_GUI_GetXSize,
    .GUI_GetYSize       = STM32_GUI_GetYSize,
    .GUI_ReadPixel      = STM32_GUI_ReadPixel,
    .GUI_DrawPixel      = STM32_GUI_DrawPixel,
    .GUI_Clear          = STM32_GUI_Clear,
    .GUI_DrawBitmap     = STM32_GUI_DrawBitmap,
    .GUI_DrawRGBImage   = STM32_GUI_DrawRGBImage
};

static uint16_t RGB888to565(uint32_t Color)
{
    uint8_t *ptr = (uint8_t*) &Color;
    
    uint8_t red   = ptr[2];
    uint8_t green = ptr[1];
    uint8_t blue  = ptr[0];
    
    uint16_t b = (blue >> 3)   & 0x1f;
    uint16_t g = ((green >> 2) & 0x3f) << 5;
    uint16_t r = ((red >> 3)   & 0x1f) << 11;
    
    return (uint16_t) (r | g | b);    
}

int32_t STM32_GUI_Init(void)
{
    BSP_LCD_Init();
    
    return 0;
}

int32_t STM32_GUI_GetXSize(void)
{
    uint32_t width = BSP_LCD_GetXSize();
    
    return width;
}

int32_t STM32_GUI_GetYSize(void)
{
    uint32_t height = BSP_LCD_GetYSize();
    
    return height;
}

int32_t STM32_GUI_ReadPixel(uint16_t Xpos, uint16_t Ypos, uint32_t *Color)
{
    uint32_t c = BSP_LCD_ReadPixel(Xpos, Ypos);
       
    *Color = RGB888to565(c);
    
    return 0;
}

int32_t STM32_GUI_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t Color)
{
    BSP_LCD_DrawPixel(Xpos, Ypos, RGB888to565(Color)); 
    
    return 0;
}

int32_t STM32_GUI_Clear(uint32_t Color)
{
    BSP_LCD_Clear(RGB888to565(Color));  
    
    return 0;
}

int32_t STM32_GUI_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
    return 0;
}

int32_t STM32_GUI_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp)
{
    return 0;
}
