#ifndef __ILI9341_SPI_H__
#define __ILI9341_SPI_H__

#include <stdint.h>
#include "gpio.h"
#include "spi.h"

#define COLOR_RED		0xF800
#define COLOR_GREEN	    0x07E0
#define COLOR_BLUE	    0x001F
#define COLOR_YELLOW    0xFFE0
#define COLOR_CYAN	    0x07FF
#define	COLOR_PURPLE    0xF81F
#define COLOR_BLACK	    0x0000
#define COLOR_WHITE	    0xFFFF

int32_t ili9341_init(void);

int32_t ili9341_get_width(void);
int32_t ili9341_get_height(void);

int32_t ili9341_clear(uint16_t color);
int32_t ili9341_bmp(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data);
int32_t ili9341_pixel(uint16_t x, uint16_t y, uint16_t color);

#endif // __ILI9341_SPI_H__
