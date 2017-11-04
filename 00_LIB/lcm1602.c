#include <stdio.h>
#include <string.h>

#include "sdk_config.h"
#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include "app_util_platform.h"
#include "app_error.h"

#include "nrf_i2c.h"
#include "lcm1602.h"

// commands
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// flags for display on/off control
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// flags for function set
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

// flags for backlight control
#define LCD_BACKLIGHT           0x08
#define LCD_NOBACKLIGHT         0x00

#define usleep(val)             nrf_delay_us(val)
#define LCM1602_ADDR            0x27

void display(void);
void home(void);
void command(char value);
void data(char value);
void write_4bits(char value);

static uint8_t _backlight = LCD_BACKLIGHT;
static uint8_t m_id       = 0;

void command(char value)
{
    uint8_t high = value & 0xF0;
    uint8_t low  = (value << 4) & 0xF0;
    
    write_4bits(high | 0x00);
    write_4bits(low  | 0x00);
}

void data(char value)
{
    char high = value & 0xF0;
    char low  = (value << 4) & 0xF0;
    
    write_4bits(high | 0x01);
    write_4bits(low  | 0x01);    
}

void write_4bits(char value)
{
    i2c_write_byte(m_id, value | _backlight);
    
    i2c_write_byte(m_id, value | 0x04 | _backlight);
    usleep(1);
    
    i2c_write_byte(m_id, value & ~(0x04) | _backlight);
    usleep(50);
}

void home(void)
{
    command(LCD_RETURNHOME);
    usleep(2000);    
}

int lcm1602_init(uint8_t id)
{
    uint8_t displayfunction = 0;
    uint8_t displaymode     = 0;
    uint8_t displaycontrol  = 0;
    
    m_id = id;
    
    i2c_init(m_id, LCM1602_ADDR);
    
    displayfunction = LCD_4BITMODE | LCD_5x8DOTS | LCD_2LINE;

    usleep(50);
    lcm1602_backlight(true);
    
    write_4bits(0x03 << 4);
    usleep(5000);
    
    write_4bits(0x03 << 4);
    usleep(5000);

    write_4bits(0x03 << 4);
    usleep(1000);

    write_4bits(0x02 << 4);

    command(LCD_FUNCTIONSET | displayfunction);
    
    displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    command(LCD_DISPLAYCONTROL | displaycontrol);    
    
    lcm1602_clear();
    
    displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  	command(LCD_ENTRYMODESET | displaymode);
	
	home();         

    return 0;
}

int lcm1602_puts(char *s)
{
    uint32_t i;
    
    if(s == 0x00)
        return -1;
    
    for(i = 0; i<strlen(s); i++) {
        data(s[i]);
    }
    
    return 0;
}

int lcm1602_clear(void)
{
    command(LCD_CLEARDISPLAY);
    usleep(2000);
    
    return 0;
}

int lcm1602_cursor(char col, char row)
{
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

	if ( row > 1 ) {
		row = 1; 
	}

	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));    
    
    return 0;
}

int lcm1602_backlight(bool enable)
{
    if(enable)
        _backlight = LCD_BACKLIGHT;
    else
        _backlight = LCD_NOBACKLIGHT;
    
    return 0;
}
