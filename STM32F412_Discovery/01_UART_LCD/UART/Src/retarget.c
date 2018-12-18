#include <stdio.h>
#include <rt_misc.h>

__asm(".global __use_no_semihosting_swi\n");

FILE __stdout;
FILE __stdin;

int ferror(FILE *f) 
{
    return EOF;
}

void _ttywrch(int c) {
    // sendchar(c);
}

void _sys_exit(int return_code) {
    label:  goto label; 
}
