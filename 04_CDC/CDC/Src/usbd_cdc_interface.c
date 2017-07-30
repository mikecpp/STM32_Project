#include "main.h"

#define APP_RX_DATA_SIZE  2048
#define APP_TX_DATA_SIZE  2048

USBD_CDC_LineCodingTypeDef LineCoding =
{
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
};

uint8_t UserRxBuffer[APP_RX_DATA_SIZE];/* Received Data over USB are stored in this buffer */
uint8_t UserTxBuffer[APP_TX_DATA_SIZE];/* Received Data over UART (CDC interface) are stored in this buffer */
uint32_t BuffLength;
uint32_t UserTxBufPtrIn  = 0;/* Increment this pointer or roll it back to
                               start address when data are received over USART */
uint32_t UserTxBufPtrOut = 0; /* Increment this pointer or roll it back to
                                 start address when data are sent over USB */

extern USBD_HandleTypeDef  USBD_Device;

static int8_t CDC_Itf_Init(void);
static int8_t CDC_Itf_DeInit(void);
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Itf_Receive(uint8_t* pbuf, uint32_t *Len);

USBD_CDC_ItfTypeDef USBD_CDC_fops = 
{
    CDC_Itf_Init,
    CDC_Itf_DeInit,
    CDC_Itf_Control,
    CDC_Itf_Receive
};

static int8_t CDC_Itf_Init(void)
{
    USBD_CDC_SetTxBuffer(&USBD_Device, UserTxBuffer, 0);
    USBD_CDC_SetRxBuffer(&USBD_Device, UserRxBuffer);
  
    return (USBD_OK);
}

static int8_t CDC_Itf_DeInit(void)
{
    return (USBD_OK);
}

static int8_t CDC_Itf_Control (uint8_t cmd, uint8_t* pbuf, uint16_t length)
{ 
    switch (cmd) {
        case CDC_SEND_ENCAPSULATED_COMMAND:
             break;

        case CDC_GET_ENCAPSULATED_RESPONSE:
             break;

        case CDC_SET_COMM_FEATURE:
             break;

        case CDC_GET_COMM_FEATURE:
             break;

        case CDC_CLEAR_COMM_FEATURE:
             break;

        case CDC_SET_LINE_CODING:
             LineCoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |\
                                     (pbuf[2] << 16)    | (pbuf[3] << 24));
             LineCoding.format     = pbuf[4];
             LineCoding.paritytype = pbuf[5];
             LineCoding.datatype   = pbuf[6];    
             break;

        case CDC_GET_LINE_CODING:
             pbuf[0] = (uint8_t)(LineCoding.bitrate);
             pbuf[1] = (uint8_t)(LineCoding.bitrate >> 8);
             pbuf[2] = (uint8_t)(LineCoding.bitrate >> 16);
             pbuf[3] = (uint8_t)(LineCoding.bitrate >> 24);
             pbuf[4] = LineCoding.format;
             pbuf[5] = LineCoding.paritytype;
             pbuf[6] = LineCoding.datatype;     
             break;

        case CDC_SET_CONTROL_LINE_STATE:
             break;

        case CDC_SEND_BREAK:
             break;    
    
        default:
             break;
    }
  
    return (USBD_OK);
}

static void PeriodElapsedCallback(void)
{
    uint32_t buffptr;
    uint32_t buffsize;
  
    if(UserTxBufPtrOut != UserTxBufPtrIn)
    {
        if(UserTxBufPtrOut > UserTxBufPtrIn) /* Roll-back */
        {
            buffsize = APP_RX_DATA_SIZE - UserTxBufPtrOut;
        }
        else 
        {
            buffsize = UserTxBufPtrIn - UserTxBufPtrOut;
        }
    
        buffptr = UserTxBufPtrOut;
    
        USBD_CDC_SetTxBuffer(&USBD_Device, (uint8_t*)&UserTxBuffer[buffptr], buffsize);
    
        if(USBD_CDC_TransmitPacket(&USBD_Device) == USBD_OK)
        {
            UserTxBufPtrOut += buffsize;
            if (UserTxBufPtrOut == APP_RX_DATA_SIZE)
            {
                UserTxBufPtrOut = 0;
            }
        }
    }
}

static int8_t CDC_Itf_Receive(uint8_t* Buf, uint32_t *Len)
{
    printf("%s \n", Buf);
    
    USBD_CDC_ReceivePacket(&USBD_Device);
    
    return (USBD_OK);
}
