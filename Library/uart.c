#include "uart.h"
#include "fifo.h"

#define MAX_TIMEOUT        1000000
#define UART_MAX_COUNT     6

#define UART_TX_TIMEOUT    1000    // 1 sec
#define UART_RX_TIMEOUT    1000    // 1 sec 

static UART_ID m_consol_id          = UART_2;            // consol port  

UART_HandleTypeDef UartHandle[UART_MAX_COUNT];

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&UartHandle[m_consol_id], (uint8_t *)&ch, 1, UART_TX_TIMEOUT);

    return ch;
}

int fgetc(FILE *f)
{
    char ch;
    
    HAL_UART_Receive(&UartHandle[m_consol_id], (uint8_t *)&ch, 1, UART_RX_TIMEOUT);

    return ch;
}    

#define MAX_BUFFER_SIZE    256

static char m_buffer_2[MAX_BUFFER_SIZE]; 
static char m_buffer_6[MAX_BUFFER_SIZE]; 

static int m_handle_2 = -1;
static int m_handle_6 = -1;

static char m_ch_2;
static char m_ch_6;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{   
    // USART2
    if(UartHandle->Instance == USART2) {
        fifo_write(m_handle_2, &m_ch_2, 1);
        if(HAL_UART_Receive_IT(UartHandle, (uint8_t *)&m_ch_2, 1) != HAL_OK) {
            printf("HAL_UART2_Receive_IT() Fail !!!\r\n");
        }
    }    
    
    // USART6
    if(UartHandle->Instance == USART6) {
        fifo_write(m_handle_6, &m_ch_6, 1);
        if(HAL_UART_Receive_IT(UartHandle, (uint8_t *)&m_ch_6, 1) != HAL_OK) {
            printf("HAL_UART6_Receive_IT() Fail !!!\r\n");
        }
    } 
}

//////////////////////////////////////////////////////////////////////////////
// Public API
//
int uart_init(UART_ID id, uint32_t baud_rate)
{   
    switch(id) {
        case UART_1:
            UartHandle[id].Instance = USART1;
            break;
        case UART_2:
            UartHandle[id].Instance = USART2;
            break;
        case UART_3:
            UartHandle[id].Instance = USART3;
            break;
#ifdef  USART6
        case UART_6:
            UartHandle[id].Instance = USART6;
            break;
#endif        
        default:
            return -1;
    }    

    UartHandle[id].Init.BaudRate     = baud_rate ;
    UartHandle[id].Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle[id].Init.StopBits     = UART_STOPBITS_1;
    UartHandle[id].Init.Parity       = UART_PARITY_NONE;
    UartHandle[id].Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle[id].Init.Mode         = UART_MODE_TX_RX;
    
    if (HAL_UART_Init(&(UartHandle[id])) != HAL_OK) {
        return -1;
    }    
    
    m_handle_2 = fifo_init(m_buffer_2, MAX_BUFFER_SIZE);
    m_handle_6 = fifo_init(m_buffer_6, MAX_BUFFER_SIZE);
    
    if((m_handle_2 < 0) || (m_handle_6 < 0)) {
        return -1; // FIFO initial fail! 
    }
    
    // Only initial USART6 Interrupt buffer
    if(id == UART_2) {
        if(HAL_UART_Receive_IT(&UartHandle[id], (uint8_t *)&m_ch_2, 1) != HAL_OK) {
            printf("HAL_UART_Receive_IT fail !!! \r\n");
        }
    }

#ifdef STM32F412Zx    
    if(id == UART_6) {
        if(HAL_UART_Receive_IT(&UartHandle[id], (uint8_t *)&m_ch_6, 1) != HAL_OK) {
            printf("HAL_UART_Receive_IT fail !!! \r\n");
        }
    }
#endif
    
    return 0;
}

int uart_consol(UART_ID id)
{
    m_consol_id = id;  
    
    return 0;
}

int uart_send(UART_ID id, uint8_t *buf, uint16_t len)
{ 
    int ret;
    
    ret = HAL_UART_Transmit(&UartHandle[id], buf, len, MAX_TIMEOUT);
    
    if(ret != HAL_OK) {
        return -1;
    }    
    
    return 0;
}

int uart_recv(UART_ID id, uint8_t *buf, uint16_t len)
{  
    if(id == UART_2) {
        while(fifo_available(m_handle_2) == 1){
            return fifo_read(m_handle_2, (char *) buf, len);
        }
    }

    if(id == UART_6) {
        while(fifo_available(m_handle_6) == 1){
            return fifo_read(m_handle_6, (char *) buf, len);
        }
    }
    
    return 0;
}

char uart_getchar(UART_ID id)
{
    char ch = 0;
    
    if(id == UART_2) {
        while(fifo_available(m_handle_2) == 1){
            fifo_read(m_handle_2, &ch, 1);
            return ch;
        }
    }

    if(id == UART_6) {
        while(fifo_available(m_handle_6) == 1){
            fifo_read(m_handle_6, &ch, 1);
            return ch;
        }
    }

    return 0;
}

int uart_putchar(UART_ID id, char ch)
{
    return HAL_UART_Transmit(&UartHandle[id], (uint8_t *)&ch, 1, UART_TX_TIMEOUT);    
}

int uart_available(UART_ID id)
{
    if(id == UART_2) { 
        return fifo_available(m_handle_2);
    }
    
    if(id == UART_6) {
        return fifo_available(m_handle_6);
    }    
    
    return 0;
}

int uart_clean(UART_ID id)
{
    if(id == UART_2) {
        return fifo_clean(m_handle_2);
    }
    
    if(id == UART_6) {
        return fifo_clean(m_handle_6);
    }    
    
    return 0;

}
