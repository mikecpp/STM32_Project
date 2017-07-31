#include "main.h"
#include "uart.h" 
#include "stm32_i2c.h"
#include "oled1602.h"

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

    i2c_register_driver(&stm32_i2c_drv);        
    oled1602_init(I2C_2);
    oled1602_cursor(0, 0);
    oled1602_puts("Hello Mike");    
    
    while (1)
    {
        sprintf(msg, "Count = %d", count++);
        oled1602_cursor(0, 1);
        oled1602_puts(msg);        
        printf("%s\r\n", msg);
        delay_ms(1000);
    }
}
