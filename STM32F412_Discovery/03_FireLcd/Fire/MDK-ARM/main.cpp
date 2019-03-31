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
}
