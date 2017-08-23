#include "main.h"
#include "uart.h" 
#include "adc.h"

int main(void)
{   
    system_init();

    if(uart_init(UART_PORT, 115200) != 0) { 
        return -1;
    }
    uart_consol(UART_PORT);
    
    printf("Hello Mike !\r\n");
    
    // Start ADC
    adc_init(ARDUINO_A0);
    adc_start(ARDUINO_A0);
    
    uint16_t adc_value;
    while (1)
    {
        adc_read(ARDUINO_A0, &adc_value); 
        printf("%d\r\n", adc_value);
        delay_ms(10);
    }
}
