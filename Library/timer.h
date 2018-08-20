#ifndef __TIMER_H__
#define __TIMER_H__

#include "main.h"
#include "stm32_timer.h"

#define MAX_TIMER_ID    5

typedef int32_t (*timer_cb)(void); 

typedef struct {
    int32_t  id;        // start ID from 0 
    timer_cb cb_func;   
    uint32_t timeout;   // uint: 10 ms
    uint32_t count;
    uint8_t  start;     // 0: STOP, 1: START 
} Timer_T;

int32_t timer_init(void);
int32_t timer_register(timer_cb func, uint32_t timeout);
int32_t timer_start(void);
int32_t timer_stop(void);
int32_t timer_delay_us(uint16_t delay);

#endif//__TIMER_H__
