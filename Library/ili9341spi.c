#include "ili9341spi.h"

#define ILI9341_WIDTH   (240)
#define ILI9341_HEIGHT  (320)

#define PIN_RS		    9
#define PIN_RST		    8
#define PIN_LED         7

#define LCD_RS_CLR      gpio_write(PIN_RS,  GPIO_LOW)
#define LCD_RS_SET      gpio_write(PIN_RS,  GPIO_HIGH)
#define LCD_RST_CLR     gpio_write(PIN_RST, GPIO_LOW)
#define LCD_RST_SET     gpio_write(PIN_RST, GPIO_HIGH)

#define LCD_WIDTH       240
#define LCD_HEIGHT      320

#define BUF_SIZE        1024

typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; 
} lcd_init_cmd_t;

static const lcd_init_cmd_t init_cmds[]={
    {0xEF, {0x03, 0x80, 0x02}, 3},
    {0xCF, {0x00, 0xC1, 0X30}, 3},
    {0xED, {0x64, 0x03, 0X12, 0X81}, 4},
    {0xE8, {0x85, 0x00, 0x78}, 3},
    {0xCB, {0x39, 0x2C, 0x00, 0x34, 0x02}, 5},
    {0xF7, {0x20}, 1},
    {0xEA, {0x00, 0x00}, 2},
    {0xC0, {0x23}, 1},
    {0xC1, {0x10}, 1},
    {0xC5, {0x3E, 0x28}, 2},
    {0xC7, {0x86}, 1},
    {0x36, {0x48}, 1},
    {0x3A, {0x55}, 1},
    {0xB1, {0x00, 0x18}, 2},
    {0xB6, {0x08, 0x82, 0x27}, 3},
    {0xF2, {0x00}, 1},
    {0x26, {0x01}, 1},
    {0xE0, {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00}, 15},
    {0XE1, {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F}, 15},
    {0, {0}, 0xff},
};

static void ili9341_cmd(uint8_t cmd)
{
    LCD_RS_CLR;
	spi_write(SPI_1, &cmd, 1);
    LCD_RS_SET;
}

static void ili9341_data(uint8_t *data, uint32_t len)
{
    LCD_RS_SET;
    spi_write(SPI_1, data, len);
    LCD_RS_CLR;
}

static void ili9341_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t data[4];
    
    ili9341_cmd(0x2A);
    data[0] = x0 >> 8;
    data[1] = x0 & 0xFF;
    data[2] = x1 >> 8;
    data[3] = x1 & 0xFF;
    ili9341_data((uint8_t *) data, 4); 
    
    ili9341_cmd(0x2B);
    data[0] = y0 >> 8;
    data[1] = y0 & 0xFF;
    data[2] = y1 >> 8;
    data[3] = y1 & 0xFF;
    ili9341_data((uint8_t *) data, 4); 
    
    ili9341_cmd(0x2C);
}

//////////////////////////////////////////////////////////////////////////////
// Public API

int32_t ili9341_init(void)
{
    // Setup GPIO PIN to output
    gpio_init(PIN_RS,  GPIO_OUTPUT);
    gpio_init(PIN_RST, GPIO_OUTPUT);
    gpio_init(PIN_LED, GPIO_OUTPUT);    
       
    spi_init(SPI_1);
    
	LCD_RST_CLR;
	delay_ms(20);	
	LCD_RST_SET;
	delay_ms(20);   
       
    int i = 0;
    while(init_cmds[i].cmd != 0x00) {
        ili9341_cmd(init_cmds[i].cmd);
        ili9341_data((uint8_t *) init_cmds[i].data, init_cmds[i].databytes);
        if(init_cmds[i].databytes & 0x80) {
            delay_ms(100);
        }
        printf("cmd = %02X\n", init_cmds[i].cmd);
        i++;
    }
    ili9341_cmd(0x11);
    delay_ms(120);
    
    ili9341_cmd(0x29);
    ili9341_cmd(0x2C);
    
    gpio_write(PIN_LED, GPIO_HIGH); // Turn on LED      
    
    return 0;
}

int32_t ili9341_get_width(void)
{
    return LCD_WIDTH;
}

int32_t ili9341_get_height(void)
{
    return LCD_HEIGHT;
}

static void ili9341_fill_color(uint16_t color, int32_t len)
{
    char data[BUF_SIZE * 2];
    int  i;
    
    while(len > 0) {
        for(i=0; i<BUF_SIZE; i++) {
            data[i*2]   = color >> 8;
            data[i*2+1] = color & 0xFF;
        }
        ili9341_data((uint8_t *) data, (len >= BUF_SIZE) ? (BUF_SIZE * 2) : (len * 2)); 
        len -= BUF_SIZE;
    }     
}

int32_t ili9341_clear(uint16_t color)
{
    ili9341_addr_window(0, 0, ILI9341_WIDTH-1, ILI9341_HEIGHT-1);
    ili9341_fill_color(color, ILI9341_WIDTH * ILI9341_HEIGHT);
    
    return 0;
}

int32_t ili9341_bmp(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data)
{
    ili9341_addr_window(x, y, x+w, y+h);   
    ili9341_data(data, w * h * 2);  

    return 0;
}

int32_t ili9341_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t data[2];
       
    ili9341_addr_window(x, y, x+1, y+1);
    
    data[0] = color >> 8;
    data[1] = color & 0xFF;
    ili9341_data((uint8_t *) data, 2);
    
    return 0;
}
