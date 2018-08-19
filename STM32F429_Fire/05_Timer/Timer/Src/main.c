#include "main.h"
#include "uart.h" 
#include "timer.h"

int task_1(void)
{
    printf("I am Task One \r\n");
    
    return 0;
}

int task_2(void)
{
    printf("I am Task Two \r\n");
    
    return 0;
}

int main(void)
{
    int count = 0;
    
    system_init();

    if(uart_init(UART_PORT, 115200) != 0) { 
        return -1;
    }
    uart_consol(UART_PORT);
    
    printf("Hello Mike !\r\n");
    
    timer_init();
    
    timer_register(task_1, 1000); // 1 second
    timer_register(task_2, 2000); // 2 seconds
       
    timer_start();
    
    while (1)
    {
        printf("Count = %d \r\n", count++);
        delay_ms(1000);
    }
}
