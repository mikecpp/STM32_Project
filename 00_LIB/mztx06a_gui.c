#include "mztx06a.h"
#include "mztx06a_gui.h"

GUI_Interface_T esp32_gui_interface = 
{
    .GUI_Init           = ESP32_GUI_Init, 
    .GUI_GetXSize       = ESP32_GUI_GetXSize,
    .GUI_GetYSize       = ESP32_GUI_GetYSize,
    .GUI_ReadPixel      = ESP32_GUI_ReadPixel,
    .GUI_DrawPixel      = ESP32_GUI_DrawPixel,
    .GUI_Clear          = ESP32_GUI_Clear,
    .GUI_DrawBitmap     = ESP32_GUI_DrawBitmap,
    .GUI_DrawRGBImage   = ESP32_GUI_DrawRGBImage
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

//////////////////////////////////////////////////////////////////////////////
// Public API

int32_t ESP32_GUI_Init(void)
{
    mztx06a_init();
    
    return 0;
}

int32_t ESP32_GUI_GetXSize(void)
{   
    return mztx06a_get_width();
}

int32_t ESP32_GUI_GetYSize(void)
{
    return mztx06a_get_height();
}

int32_t ESP32_GUI_ReadPixel(uint16_t Xpos, uint16_t Ypos, uint32_t *Color)
{
    return 0;
}

int32_t ESP32_GUI_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t Color)
{
    mztx06a_pixel(Xpos, Ypos, RGB888to565(Color));
    
    return 0;
}

int32_t ESP32_GUI_Clear(uint32_t Color)
{
    mztx06a_clear(RGB888to565(Color));
    
    return 0;
}

int32_t ESP32_GUI_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
    return 0;
}

int32_t ESP32_GUI_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp)
{
    mztx06a_bmp(Xpos, Ypos, Xsize, Ysize, pbmp);
    
    return 0;
}
