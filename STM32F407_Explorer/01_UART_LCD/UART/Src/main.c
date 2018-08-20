#include "main.h"
#include "uart.h" 
#include "stm32f407_explorer_lcd.h" 
#include <stdlib.h>
#include <time.h>

#define X_SIZE      800
#define Y_SIZE      480

static void pixel_test(void)
{   
    uint16_t x, y, w, h;
    uint16_t c;
    
    LCD_Clear(LCD_COLOR_WHITE);   

    while(1) {
        x = rand() % X_SIZE;
        y = rand() % Y_SIZE;
        w = rand() % X_SIZE;
        y = rand() % Y_SIZE;
        c = rand() % 0xFFFF;
		LCD_SetTextColor(c);
        LCD_FillRect(x, y, 100, 100);
        delay_ms(10);
    }
}

static void h_line_test(void)
{
    uint16_t x, y, w;
    uint16_t c;
    
	LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_Clear(LCD_COLOR_BLACK);   

    while(1) {
        x = rand() % X_SIZE;
        y = rand() % Y_SIZE;
        w = rand() % X_SIZE;
        c = rand() % 0xFFFF;
		LCD_SetTextColor(c);
        LCD_DrawHLine(x, y, w);
        delay_ms(10);
    }		
}

static void v_line_test(void)
{
    uint16_t x, y, w;
    uint16_t c;
    
	LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_Clear(LCD_COLOR_BLACK);   

    while(1) {
        x = rand() % X_SIZE;
        y = rand() % Y_SIZE;
        w = rand() % X_SIZE;
        c = rand() % 0xFFFF;
		LCD_SetTextColor(c);
        LCD_DrawVLine(x, y, w);
        delay_ms(10);
    }		
}

static void rectangle_test(void)
{
    uint16_t x, y, w, h;
    uint16_t c;
    
	LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_Clear(LCD_COLOR_BLACK);   

    while(1) {
        x = rand() % X_SIZE;
        y = rand() % Y_SIZE;
        w = rand() % X_SIZE;
        h = rand() % Y_SIZE;
        c = rand() % 0xFFFF;
		LCD_SetTextColor(c);
		w = 100;
		h = 100;
        LCD_DrawRect(x, y, w, h);
        delay_ms(10);
    }		
}

/*
static void rectangle_test(void)
{
}

static void circle_text(void)
{
}
*/

int main(void)
{
    // int count = 0;
    char msg[128];
    
    system_init(); 

    if(uart_init(UART_PORT, 115200) != 0) { 
        return -1;
    }
    
    uart_consol(UART_PORT);
    printf("Hello Mike !\r\n");
    
    LCD_Init();

    LCD_Clear(LCD_COLOR_BLACK);
     
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_SetBackColor(LCD_COLOR_YELLOW);      
    LCD_DisplayStringAt(0, 0,  (uint8_t*) "NT35510 Demo", CENTER_MODE);

    LCD_SetTextColor(LCD_COLOR_YELLOW);
    LCD_SetBackColor(LCD_COLOR_BLACK);  
    LCD_DisplayStringAtLine(1, (uint8_t*) "Hello Mike");
    LCD_DisplayStringAtLine(2, (uint8_t*) "ABCDE1234567890");
    
    uint32_t x_size = 0, y_size = 0; 
    
    x_size = LCD_GetXSize();
    y_size = LCD_GetYSize();
    sprintf(msg, "x_size = %d, y_size = %d", x_size, y_size);
    LCD_DisplayStringAtLine(3, (uint8_t *) msg);   
    
    delay_ms(1000);
    
    // pixel_test();
	// h_line_test();
	// v_line_test();
	rectangle_test();
    
    /*
    while (1)
    {
        sprintf(msg, "Count = %d", count++);
        LCD_DisplayStringAtLine(15, (uint8_t *) msg);
        delay_ms(1000);
    }
    */
}
