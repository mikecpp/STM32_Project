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

#ifdef USE_STM32F429I_DISCO
    #include "stm32f429_system.h" 
    #define UART_PORT   UART_1 
#endif 

#if defined(USE_STM32F407_MINI) || defined(USE_STM32F407_EXPLORER)
    #include "stm32f407_system.h" 
    #define UART_PORT   UART_1     
#endif

#endif//__MAIN_H 
