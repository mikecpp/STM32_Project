#include "main.h"
#include "uart.h" 
#include "stm32412g_discovery_lcd.h"

#include <list>
#include <iterator>

void list_test()
{
    std::list<int> list_int;
    std::list<int>::iterator it;

    list_int.push_back(100);
    list_int.push_back(200);
    list_int.push_back(300);

    it = list_int.begin();
	
	while(it != list_int.end()) {
        printf("%d \r\n", *it);
        it++;
    }    
}

int main(void)
{
    system_init(); 
    if(uart_init(UART_PORT, 115200) != 0) { 
        return -1;
    }   
    uart_consol(UART_PORT);   
    printf("Hello Mike !\r\n");
    
    list_test();
    
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
