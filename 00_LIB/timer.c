#include "main.h"
#include "timer.h"

static Timer_T m_timer[MAX_TIMER_ID];
static int8_t m_id    = 0;
static int8_t m_empty = 1;

static void timer_entry(void)
{
    int i;
    
    if(m_empty) { // Empty 
        return;
    }    
    
    for(i=0; i<m_id; i++) {
        if(m_timer[i].start == 1) { 
            m_timer[i].count--; 
            if(m_timer[i].count <= 0) {
                m_timer[i].cb_func();
                m_timer[i].count = m_timer[i].timeout;
            }    
        }    
    }    
}

int32_t timer_init(void)
{
    return stm32_timer_init(timer_entry);
}

int32_t timer_register(timer_cb func, uint32_t timeout)
{
    if(m_id >= MAX_TIMER_ID) { 
        return -1;
    }    
    
    m_empty = 0;
    
    m_timer[m_id].id      = m_id; 
    m_timer[m_id].cb_func = func;
    m_timer[m_id].timeout = timeout;
    m_timer[m_id].count   = timeout;
    m_timer[m_id].start   = 1;
    
    m_id++;
    
	return 0;
}

int32_t timer_start(void)
{
    stm32_timer_start();
    
    return 0;
}

int32_t timer_stop(void)
{
    stm32_timer_stop();

    return 0;
}

int32_t timer_delay_us(uint16_t delay)
{
    stm32_timer_delayus(delay); 
    
    return 0;
}
