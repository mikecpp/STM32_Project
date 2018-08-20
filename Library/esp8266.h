#ifndef __ESP8266_H__
#define __ESP8266_H__

#include <stdint.h>

typedef enum {
    WIFI_STATION = 1,
    WIFI_AP,
    WIFI_AP_STATION
} WIFI_MODE_T;

typedef enum {
    IP_TCP,
    IP_UDP
} IP_PROTOCOL_T;

typedef enum { 
    IP_STATUS_GET_IP       = 2,
    IP_STATUS_CONNECTED    = 3,
    IP_STATUS_DISCONNECTED = 4
} IP_CONNECT_STATUS_T;

typedef int (*esp8266_func_t)(char *data, int len); 

int esp8266_init(int8_t uart_id);
int esp8266_send(char *data, int len);
int esp8266_recv(char *data, int len);
int esp8266_cmd(char *command, char *reply, int timeout);

int esp8266_reset(void);
int esp8266_at(void);
int esp8266_set_wifi_mode(WIFI_MODE_T mode); 
int esp8266_join_ap(char *ssid, char *pwd);
int esp8266_set_ap(char *ssid, char *pwd);
int esp8266_ip_connect(IP_PROTOCOL_T protocol, char *server, char *port, char id);
int esp8266_ip_link_status(void);
int esp8266_ip_link_id(char *id);
int esp8266_ip_get_ip(char *ip, int len);
int esp8266_ip_server(char *port);

int esp8266_ip_send(int id, char *data, int len);
int esp8266_ip_recv(int id, char *data, int len);

#endif//__ESP8266_H__
