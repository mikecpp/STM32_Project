#include "main.h" 
#include "uart.h" 

USBD_HandleTypeDef USBD_Device;  

int main(void)
{
    int count = 0;
    
    system_init();

    if(uart_init(UART_PORT, 115200) != 0) { 
        return -1;
    }
    uart_consol(UART_PORT);
    
    USBD_Init(&USBD_Device, &VCP_Desc, 0);
    USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
    USBD_Start(&USBD_Device);    
    
    printf("Hello Mike !\r\n");
    
    while (1)
    {
        printf("Count = %d \r\n", count++);
        delay_ms(1000);
    }
}
