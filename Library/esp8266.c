#include <stdio.h>
#include <string.h>

#include "esp8266.h"
#include "stm32f412_system.h"
#include "uart.h"

#define DEBUG                      0

#define MAX_CMD_LEN               80
#define MAX_RECV_DATA_LEN        512

#define WAIT_SHORT               100   //  ms

typedef enum {
    ESP8266_STATE_COMMAND = 0,
    ESP8266_STATE_CONNECTED
} ESP8266_STATE;

typedef enum {
    ESP8266_OK = 0,
    ESP8266_ERROR,
    ESP8266_CONNECT, 
    ESP8266_CLOSED,
    ESP8266_IPD,
    ESP8266_UNKOWN
} ESP8266_REPLY;

static char m_cmd[MAX_CMD_LEN];    
static int  m_uart_id = -1;

static char m_recv_data[MAX_RECV_DATA_LEN];

//////////////////////////////////////////////////////////////////////////////
// Private Functions

int esp8266_send(char *data, int len)
{   
    // Clean previuos received UART buffer  
    uart_clean((UART_ID) m_uart_id); 
    
    return uart_send((UART_ID) m_uart_id, (uint8_t*) data, len); 
}

int esp8266_recv(char *data, int len)
{
    char ch;
    int  i = 0;
   
    while((ch = uart_getchar((UART_ID) m_uart_id)) > 0) {
        data[i++] = ch;
        if(i >= len) {
            i = len;
            #if DEBUG
            printf("ESP8266 data buffer overflow !\r\n");
            #endif
            break;
        }
    }     
    
    return i;
} 

int esp8266_cmd(char *command, char *reply, int timeout)
{
    int ret = 0;
    
    ret = esp8266_send(command, strlen(command));  
    
    #if DEBUG
    printf("---> %s", command);
    #endif
    
    if(ret != 0) {
        return -1;
    }
    
    delay_ms(timeout);
    
    ret = esp8266_recv(m_recv_data, MAX_RECV_DATA_LEN); 
    
    if(ret <= 0) {
        return -1;
    }
    m_recv_data[ret] = 0x00;
    
    #if DEBUG
    printf("<--- %s", m_recv_data);
    #endif
    
    if(strstr(m_recv_data, reply)) {
        return 0; // OK
    }
    
    return 1; // ERROR
}

//////////////////////////////////////////////////////////////////////////////
// Public API

int esp8266_init(int8_t uart_id)
{
    m_uart_id = uart_id;
    uart_init((UART_ID) m_uart_id, 115200);
    
    esp8266_reset();
    
    return 0;
}

int esp8266_reset(void)
{
    return esp8266_cmd("AT+RST\r\n", "OK", 3000);   
}

int esp8266_at(void)
{
    return esp8266_cmd("AT\r\n", "OK", WAIT_SHORT);
}

int esp8266_set_wifi_mode(WIFI_MODE_T mode)
{
    switch(mode) {
        case WIFI_STATION:
            return esp8266_cmd("AT+CWMODE=1\r\n", "OK", 2000);
        case WIFI_AP:
            return esp8266_cmd("AT+CWMODE=2\r\n", "OK", 2000);
        case WIFI_AP_STATION:
            return esp8266_cmd("AT+CWMODE=3\r\n", "OK", 2000);
        default:
            return -1;
    }    
}

int esp8266_join_ap(char *ssid, char *pwd)
{  
    sprintf(m_cmd, "AT+CWJAP=%s,%s\r\n", ssid, pwd);

    return esp8266_cmd(m_cmd, "OK", 5000);
}

int esp8266_set_ap(char *ssid, char *pwd)
{
    sprintf(m_cmd, "AT+CWSAP=%s,%s\r\n", ssid, pwd);

    return esp8266_cmd(m_cmd, "OK", 5000);
}

int esp8266_ip_connect(IP_PROTOCOL_T protocol, char *server, char *port, char id)
{
    #define STR_LEN   64
    char str[STR_LEN];
    
    switch(protocol) {
        case IP_TCP:
            sprintf(str, "\"%s\", \"%s\",%s", "TCP", server, port);
            break;
        case IP_UDP:
            sprintf(str, "\"%s\", \"%s\",%s", "UCP", server, port);
            break;
        default:
            break;
    }
    
    if(strlen(str) >= STR_LEN) {
        return -1;
    }    
    
    if(id < 5) {
        sprintf(m_cmd, "AT+IPSTART=%d,%s\r\n", id, str);
    }
    else {
        sprintf(m_cmd, "AT+IPSTART=%s\r\n", str);
    }
       
    return esp8266_cmd(m_cmd, "OK", 3000);
}

int esp8266_ip_link_status(void)
{
    if(esp8266_cmd("AT+CIPSTATUS\r\n", "OK", WAIT_SHORT) != 0) {
        return -1;
    } 
    
    if(strstr(m_recv_data, "STATUS:2\r\n")) {
        return IP_STATUS_GET_IP;        // Get IP (2)
    }
    
    if(strstr(m_recv_data, "STATUS:3\r\n")) {
        return IP_STATUS_CONNECTED;     // CONNECTED (3)
    }

    if(strstr(m_recv_data, "STATUS:4\r\n")) {
        return IP_STATUS_DISCONNECTED;  // DISCONNECTED (4)
    }

    return 0;
}

int esp8266_ip_link_id(char *id)
{
    *id = 0x00;
    
    if(esp8266_cmd("AT+CIPSTATUS", "OK", WAIT_SHORT) != 0) {
        return -1;
    } 
    
    if(strstr(m_recv_data, "+CIPSTATUS:0\r\n")) {
        *id |= 0x01;
    }
    
    if(strstr(m_recv_data, "+CIPSTATUS:1\r\n")) {
        *id |= 0x02;
    }
    
    if(strstr(m_recv_data, "+CIPSTATUS:2\r\n")) {
        *id |= 0x04;
    }
    
    if(strstr(m_recv_data, "+CIPSTATUS:3\r\n")) {
        *id |= 0x08;
    }
    
    if(strstr(m_recv_data, "+CIPSTATUS:4\r\n")) {
        *id |= 0x10;
    }
    
    return 0;
}

int esp8266_ip_get_ip(char *ip, int len)
{
    int i;
    char *p;
    
    if(esp8266_cmd("AT+CIFSR\r\n", "OK", WAIT_SHORT) != 0) {
        return -1;
    }     
    
    p = strstr(m_recv_data, "APIP,\"");
    
    if(p == NULL) {
        return -1;
    }

    for(i=0; i<len; i++) {
        ip[i] = *(p+i);
        if(ip[i] == '\"') {
            ip[i] = 0x00;
            break;
        }
    }
    
    return 0;
}

int esp8266_ip_server(char *port)
{
    esp8266_cmd("AT+CIPMUX=1\r\n", "OK", WAIT_SHORT);

    sprintf(m_cmd, "AT+CIPSERVER=%d,%s\r\n", 1, port);
    esp8266_cmd(m_cmd, "OK", WAIT_SHORT);
    
    return 0;   
}

int esp8266_ip_send(int id, char *data, int len)
{   
    // IP SEND 
    if(id < 5) {
        sprintf(m_cmd, "AT+CIPSEND=%d,%d\r\n", id, len);
    }
    else {
        sprintf(m_cmd, "AT+CIPSEND=%d\r\n", len);
    }
    
    esp8266_cmd(m_cmd, ">", WAIT_SHORT);
    esp8266_cmd(data, "SEND OK", WAIT_SHORT);
    
    return 0;   
}

int esp8266_ip_recv(int id, char *data, int len)
{
    int ret = 0;
    
    ret = esp8266_recv(m_recv_data, MAX_RECV_DATA_LEN);
    if(ret <= 0) {
        return -1;
    }    
    
    if(strstr(m_recv_data, "+IPD")) {
        if(ret >= len) {
            ret = len;
        }    
        memcpy(data, m_recv_data, ret);
        return ret;
    }    
    
    return 0;   
}
