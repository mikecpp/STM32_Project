#include "ili9341spi.h"
#include "ili9341spi_gui.h"

GUI_Interface_T ili9341_gui_interface = 
{
    .GUI_Init           = ILI9341_GUI_Init, 
    .GUI_GetXSize       = ILI9341_GUI_GetXSize,
    .GUI_GetYSize       = ILI9341_GUI_GetYSize,
    .GUI_ReadPixel      = ILI9341_GUI_ReadPixel,
    .GUI_DrawPixel      = ILI9341_GUI_DrawPixel,
    .GUI_Clear          = ILI9341_GUI_Clear,
    .GUI_DrawBitmap     = ILI9341_GUI_DrawBitmap,
    .GUI_DrawRGBImage   = ILI9341_GUI_DrawRGBImage
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

int32_t ILI9341_GUI_Init(void)
{
    ili9341_init();
    
    return 0;
}

int32_t ILI9341_GUI_GetXSize(void)
{   
    return ili9341_get_width();
}

int32_t ILI9341_GUI_GetYSize(void)
{
    return ili9341_get_height();
}

int32_t ILI9341_GUI_ReadPixel(uint16_t Xpos, uint16_t Ypos, uint32_t *Color)
{
    return 0;
}

int32_t ILI9341_GUI_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t Color)
{
    ili9341_pixel(Xpos, Ypos, RGB888to565(Color));
    
    return 0;
}

int32_t ILI9341_GUI_Clear(uint32_t Color)
{
    ili9341_clear(RGB888to565(Color));
    
    return 0;
}

int32_t ILI9341_GUI_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
    return 0;
}

int32_t ILI9341_GUI_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp)
{
    ili9341_bmp(Xpos, Ypos, Xsize, Ysize, pbmp);
    
    return 0;
}
