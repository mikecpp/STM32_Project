#include "main.h"
#include "uart.h" 
#include "stm32_gui.h"

int main(void)
{
    int count = 0;
    char msg[32];
    
    system_init();

    if(uart_init(UART_PORT, 115200) != 0) { 
        return -1;
    }
    uart_consol(UART_PORT);
    
    printf("Hello Mike !\r\n");
    
    GUI_Register(&stm32_gui_interface); 
    GUI_Init();

    GUI_Clear(GUI_COLOR_BLACK); 
    GUI_SetBackColor(GUI_COLOR_BLACK);
    GUI_SetTextColor(GUI_COLOR_BLUE);
    GUI_DisplayStringAt(30, 30, (uint8_t*) "Hello", GUI_CENTER_MODE); 
    GUI_SetTextColor(GUI_COLOR_YELLOW);
    GUI_DisplayStringAtLine(3, (uint8_t*) "Hello Mike");

    GUI_DrawRect(10, 10, 100, 100);
    GUI_FillEllipse(100, 150, 50, 50);

    while (1)
    {
        sprintf(msg, "Count = %d", count++); 
        printf("%s\r\n", msg);
        GUI_DisplayStringAtLine(5, (uint8_t*) msg);
        delay_ms(1000);
    }
}
