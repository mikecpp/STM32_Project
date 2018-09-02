#include "stm32f469_system.h" 
#include "uart.h"

int main(void)
{
    system_init();
    
    uart_init(UART_3, 115200);
    uart_consol(UART_3);
    
    printf("Hello World !\r\n");
 
    int  count = 0;
    while (1) {
        printf("count = %d \r\n", count++);
        delay_ms(1000);
    }
}
