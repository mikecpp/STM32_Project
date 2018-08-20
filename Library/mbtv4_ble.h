#ifndef __MBTV4_BLE_H__
#define __MBTV4_BLE_H__

#include "uart.h"

typedef enum {
    BLE_MODE_COMMAND,
    BLE_MODE_DATA
} MBTV4_BLE_MODE_T;

typedef void (*func_recv_t)(uint8_t *data, uint16_t len);

int mbtv4_ble_init(UART_ID id, func_recv_t func);
int mbtv4_ble_mode(void);

int mbtv4_ble_send(uint8_t *data, uint16_t len);
int mbtv4_ble_recv(uint8_t *data, uint16_t len);

void mbtv4_ble_task(void);

#endif//__MBTV4_BLE_H__
