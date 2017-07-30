#ifndef __MAIN_H
#define __MAIN_H

#ifdef USE_STM32412G_DISCOVERY
    #include "stm32f412_system.h"
    #define UART_PORT   UART_2
#endif    

#ifdef USE_STM32F723E_DISCOVERY
    #include "stm32f723_system.h"
    #define UART_PORT   UART_6
#endif    

#ifdef USE_STM32F429_FIRE
    #include "stm32f429_system.h" 
    #define UART_PORT   UART_1
#endif 

#ifdef USE_STM32F103_FIRE
    #include "stm32f103_system.h" 
    #define UART_PORT   UART_1 
#endif 

#include "usbd_core.h"  
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"

#endif//__MAIN_H 
