#include <string.h>
#include "mbtv4_ble.h"

#define MAX_RECV_DATA_LEN        256

static UART_ID m_id  = UART_6;
static int m_mode    = BLE_MODE_COMMAND;
static char m_recv_data[MAX_RECV_DATA_LEN];

func_recv_t m_func_recv = NULL;

int mbtv4_ble_init(UART_ID id, func_recv_t func)
{
    m_id = id; 
    m_func_recv = func;
    
    if(uart_init((UART_ID) m_id, 115200) != 0) {
        return -1;
    }
    
    return 0;
}

int mbtv4_ble_mode(void)
{
    return m_mode;
}

int mbtv4_ble_send(uint8_t *data, uint16_t len)
{
    int ret = 0;
    
    if(m_mode == BLE_MODE_DATA) {
        ret = uart_send(m_id, data, len);
    }
    
    return ret;
}

int mbtv4_ble_recv(uint8_t *data, uint16_t len)
{
    return uart_recv(m_id, data, len);   
}

void mbtv4_ble_task(void)
{
    int ret;
    
    ret = uart_recv(m_id, (uint8_t *) m_recv_data, MAX_RECV_DATA_LEN);
    
    if(ret == 0) {
        return;
    }    
            
    m_recv_data[ret] = 0x00;  
    
    if(strstr(m_recv_data, "+CONNECTED") && (m_mode == BLE_MODE_COMMAND)) {
        m_mode = BLE_MODE_DATA;
        printf("BLE MODE: CONNECTED\r\n");
    }    
        
    if(strstr(m_recv_data, "+PAIRABLE") && (m_mode == BLE_MODE_DATA)) {
        m_mode = BLE_MODE_COMMAND; 
        printf("BLE MODE: DISCONNECTED\r\n");
    }  
    
    if(m_func_recv && (m_mode == BLE_MODE_DATA) && (m_recv_data[0] != '+')) {
        m_func_recv((uint8_t *) m_recv_data, ret);
    }    
     
}
