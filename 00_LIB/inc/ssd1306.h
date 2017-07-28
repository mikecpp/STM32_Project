#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "i2c.h"
#include "string.h"

#define SSD1306_BLACK       0
#define SSD1306_WHITE       1
#define SSD1306_INVERSE     2

int ssd1306_init(char i2c_id);
void ssd1306_display(void);
void ssd1306_clear(void);
void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color); 
void ssd1306_putc(uint8_t x, uint8_t y, char c);
void ssd1306_puts(uint8_t x, uint8_t y, char* str);

#endif//__SSD1306_H__
