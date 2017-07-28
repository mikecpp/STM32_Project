#include "main.h"
#include "uart.h"

#ifdef USE_STM32412G_DISCOVERY
    #include "stm32f412_system.h"
    #define UART_PORT   UART_2
#endif    

#ifdef USE_STM32F723E_DISCOVERY
    #include "stm32f723_system.h"
    #define UART_PORT   UART_6
#endif    

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
