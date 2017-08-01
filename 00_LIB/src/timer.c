#include "main.h"
#include "timer.h"

static Timer_T m_timer[MAX_TIMER_ID];
static int8_t m_id = -1;

static void do_timer(void)
{
    int i;

    if(m_id == -1) { // Empty 
        return;
    }   
    
    for(i=0; i<m_id; i++) {
        if(m_timer[i].count == 0) {
            m_timer[i].cb_func();
            m_timer[i].count = m_timer[i].timeout;
        }
    }
}

static void timer_entry(void)
{
    int i;
    
    if(m_id == -1) { // Empty 
        return;
    }    
    
    for(i=0; i<m_id; i++) {
        if(m_timer[i].start == 1) { 
            m_timer[i].count--; 
            if(m_timer[i].count <= 0) {
                m_timer[i].count = 0;
            }    
        }    
    }
    
    do_timer();
}

TIM_HandleTypeDef        TimHandle;

int32_t timer_init(void)
{
    
    
    
	return 0;
}

int32_t timer_register(timer_cb func, uint32_t timeout)
{
    if(m_id >= MAX_TIMER_ID) { 
        return -1;
    }    
    
    m_timer[m_id].id      = m_id; 
    m_timer[m_id].cb_func = func;
    m_timer[m_id].timeout = timeout;
    m_timer[m_id].count   = timeout;
    m_timer[m_id].start   = 0;
    
    m_id++;
    
	return m_timer[m_id].id;
}

int32_t timer_start(int32_t id)
{
    if((id < 0)|| (id >= MAX_TIMER_ID)) {
        return -1;
    }

    m_timer[id].start = 1;
    
	return 0;
}

int32_t timer_stop(int32_t id)
{
    if((id < 0)|| (id >= MAX_TIMER_ID)) {
        return -1;
    }

    m_timer[id].start = 0;

	return 0;
}
