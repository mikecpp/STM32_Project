#ifndef __LCM1602_H__
#define __LCM1602_H__

#include <stdint.h>
#include <stdbool.h>

int lcm1602_init(uint8_t id);
int lcm1602_puts(char *s);
int lcm1602_clear(void);
int lcm1602_cursor(char col, char row);
int lcm1602_backlight(bool enable);

#endif//__LCM_1602_H__
