#include "main.h"
#include "uart.h" 
#include "FireSsd1306.h"

FireSsd1306 lcd(I2C_2);
    
void lcd_test()
{
    lcd.drawHLine(0, 16, 127);
    lcd.drawVLine(10, 0, 63); 
    lcd.drawVLine(117, 0, 63); 
    lcd.drawLine(0, 0, 127, 63);
    lcd.drawLine(0, 63, 127, 0);

    lcd.drawRect(0, 0, 127, 63);
    lcd.drawCircle(63, 31, 20);
    lcd.drawEllipse(63, 32, 30, 20);
}

int main(void)
{
    system_init(); 
    if(uart_init(UART_PORT, 115200) != 0) { 
        return -1;
    }   
    uart_consol(UART_PORT);   
    printf("Hello Mike !\r\n");

    lcd.init();
    lcd_test();
    lcd.displayStringAt(0, 0, (uint8_t*) "Hello Mike", LCD_CENTER_MODE);    
    lcd.update();    
}
