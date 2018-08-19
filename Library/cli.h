#ifndef __CLI_H__
#define __CLI_H__

#include "uart.h"

typedef int (*cli_func)(int argc, char *argv[]);

int cli_init(UART_ID id);
int cli_register(cli_func func, char *cmd, char *help); 
int cli_exec(char *string);
void cli_process(void); 

#endif//__CLI_H__
