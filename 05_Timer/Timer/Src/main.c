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
    int32_t id_1, id_2;
    
    system_init();

    if(uart_init(UART_PORT, 115200) != 0) { 
        return -1;
    }
    uart_consol(UART_PORT);
    
    printf("Hello Mike !\r\n");
    
    timer_init();
    
    id_1 = timer_register(task_1, 100); // 1 second
    id_2 = timer_register(task_2, 500); // 5 seconds
    
    timer_start(id_1);  
    timer_start(id_2);
    
    while (1)
    {
        printf("Count = %d \r\n", count++);
        delay_ms(1000);
    }
}
