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
    
    char msg[32];
    while (1)
    {
        sprintf(msg, "Count = %d \r\n", count++);
        USBD_CDC_SetTxBuffer(&USBD_Device, (uint8_t*) &msg, strlen(msg));  
        USBD_CDC_TransmitPacket(&USBD_Device);
        printf("%s", msg);
        delay_ms(1000);
    }
}
