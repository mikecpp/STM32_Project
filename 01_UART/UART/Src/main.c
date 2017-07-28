#include "main.h"
#include "uart.h" 

int main(void)
{
    int count = 0;
    
    system_init();

    if(uart_init(UART_PORT, 115200) != 0) {
        return -1;
    }
    uart_consol(UART_PORT);
    
    printf("Hello Mike !\r\n");
    
    while (1)
    {
        printf("Count = %d \r\n", count++);
        delay_ms(1000);
    }
}
