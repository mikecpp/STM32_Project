#include "main.h"
#include "uart.h"
#include "stm32_i2c.h" 
#include "oled1602.h"

#ifdef USE_STM32412G_DISCOVERY
    #include "stm32f412_system.h"
    #define UART_PORT   UART_2
    #define I2C_PORT    I2C_2
#endif    

#ifdef USE_STM32F723E_DISCOVERY
    #include "stm32f723_system.h"
    #define UART_PORT   UART_6
    #define I2C_PORT    I2C_2
#endif    

int main(void)
{
    int count = 0;
    char msg[32];    
    
    system_init();

    if(uart_init(UART_PORT, 115200) != 0) {
        return -1;
    }
    uart_consol(UART_PORT);
    
    i2c_register_driver(&stm32_i2c_drv);     
    oled1602_init(I2C_PORT);
    
    printf("Hello Mike !\r\n");
    oled1602_cursor(0, 0);
    oled1602_puts("Hello Mike");
    
    while (1)
    {
        printf("count = %d \r\n", count++);
        sprintf(msg, "Count = %d", count);
        oled1602_cursor(0, 1);
        oled1602_puts(msg);
        delay_ms(1000);
    }
}
