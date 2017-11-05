#include "main.h"

#include "stm32_spi.h"
#include "spi.h"
#include "stm32_gpio.h"
#include "gpio.h"
#include "uart.h" 
#include "ili9341spi_gui.h"
#include "gui.h"

int main(void)
{
    system_init();
    
    if(uart_init(UART_PORT, 115200) != 0) { 
        return -1;
    }
    uart_consol(UART_PORT);    

    gpio_register_driver(&stm32_gpio_drv);  
    spi_register_driver(&stm32_spi_drv);
    GUI_Register(&ili9341_gui_interface);    
   
    GUI_Init();
    GUI_Clear(GUI_COLOR_BLACK);     
    
    GUI_SetBackColor(GUI_COLOR_BLACK);
    GUI_SetTextColor(GUI_COLOR_YELLOW);
    GUI_DisplayStringAt(0, 0, (uint8_t*) "Hello", GUI_CENTER_MODE); 
    GUI_DisplayStringAtLine(3, (uint8_t*) "Hello Mike");

    int count = 0;
    char msg[32];    
    
    while (1)
    {
        sprintf(msg, "Count = %d", count++); 
        GUI_DisplayStringAtLine(5, (uint8_t*) msg);
        delay_ms(1000);
    }
}
