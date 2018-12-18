#include "main.h"
#include "uart.h" 
#include "stm32412g_discovery_lcd.h"

int main(void)
{
    system_init(); 
    
    if(uart_init(UART_PORT, 115200) != 0) { 
        return -1;
    }   
    uart_consol(UART_PORT);   
    printf("Hello Mike !\r\n");
    
    LCD_Init(); 
    LCD_SetFont(&Font24); 
    
    LCD_Clear(LCD_COLOR_BLACK);  
     
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_SetBackColor(LCD_COLOR_YELLOW);      
    LCD_DisplayStringAt(0, 0,  (uint8_t*) "NT35510 Demo", CENTER_MODE);

    LCD_SetTextColor(LCD_COLOR_YELLOW);
    LCD_SetBackColor(LCD_COLOR_BLACK);  
    LCD_DisplayStringAtLine(1, (uint8_t*) "Hello Mike");
    LCD_DisplayStringAtLine(2, (uint8_t*) "ABCDE1234567890");
    
}
