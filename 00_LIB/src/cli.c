#include <stdio.h>
#include <string.h>

#include "cli.h"

#define MAX_CLI_COUNT        (20)
#define MAX_STR_LEN          (64)
#define MAX_PARAM            (10)
#define CMD_LEN              (16)
#define HELP_LEN             (64)
#define CLI_PROMPT           "> "
#define MSG_NOTFOUND         "Error: Command not found ! Use help! \r\n"

#define CMD_EXIT    "exit"

typedef struct {
    cli_func func;
    char cmd[CMD_LEN];
    char help[HELP_LEN];
} cli_cmd_t;

static int cli_parse_str(char *str, char *param[]);
static int cli_help(int argc, char *argv[]);

static cli_cmd_t m_cmd_table[MAX_CLI_COUNT];
UART_ID          m_uart_id;
static int       m_count = 0;

#define CLI_PRINT(fmt, args...) fprintf(stdout, fmt, ##args) 
#define CLI_GETCH(x)            cli_getch()

static int cli_getch(void)
{
    while(uart_available(m_uart_id)) {
        return uart_getchar(m_uart_id);    
    }
    
    return 0;
}

static int cli_parse_str(char *str, char *param[]) 
{
    int count = 0;
    char *ptr = NULL;
    
    ptr = strtok(str, " ");
    while(ptr != NULL) {
        param[count++] = ptr;
        ptr = strtok(NULL, " ");
        if(count >= MAX_PARAM) {
            break;
        }    
    }
    
    return count;
}

static int cli_help(int argc, char *arg[])  
{
    cli_cmd_t *ptr = m_cmd_table;

    CLI_PRINT("command:\r\n");
    while(ptr->cmd[0]) {
        CLI_PRINT("%-16s: %s\r\n", ptr->cmd, ptr->help);
        ptr++;
    }
    
    return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Public API
//

int cli_init(UART_ID id)
{
    m_uart_id = id;
    memset(m_cmd_table, 0x00, sizeof(m_cmd_table));
    
    cli_register(cli_help, "help", "list all command"); 
    
    return 0;
}

int cli_register(cli_func func, char *cmd, char *help) 
{
    if(m_count >= MAX_CLI_COUNT) {
        return -1;
    }    
    
    cli_cmd_t *ptr = (cli_cmd_t *) &m_cmd_table[m_count]; 
    
    if((func == NULL) || (strlen(cmd) > CMD_LEN) ||
        (strlen(help) > HELP_LEN)) {
        return -2;
    }        
    
    ptr->func = func;
    strcpy(ptr->cmd, cmd);
    strcpy(ptr->help, help);
    
    m_count++;
    
    return 0;
}

int cli_exec(char *string)
{
    cli_cmd_t *ptr = NULL;
    int count = 0; 
    char *param[MAX_PARAM]; 
    
    if(*string == '\0') {
        return 0;
    }
    
    ptr   = m_cmd_table;
    count = cli_parse_str(string, param);  
    
    while(ptr->cmd[0]) 
    {
        if(strcmp(param[0], ptr->cmd) == 0)
        {
            ptr->func(count, param);  
            break;
        }
        ptr++;
    }
        
    if(ptr->cmd[0] == 0x00) {
        CLI_PRINT(MSG_NOTFOUND);
    }
    
    return 0;
}

typedef enum {
    CLI_STATE_INIT,
    CLI_STATE_INPUT,
    CLI_STATE_EXEC
} CLI_State_T;

CLI_State_T cli_state = CLI_STATE_INIT; 

void cli_process(void)  
{
    static int i = 0;
    static char cmd[MAX_STR_LEN];
    char ch;

    switch(cli_state) { 
        case CLI_STATE_INIT:
             CLI_PRINT(CLI_PROMPT);
             cli_state = CLI_STATE_INPUT;
             break;
        
        case CLI_STATE_INPUT:
             ch = CLI_GETCH(); 
             switch (ch) {
                 case '\0':
                     break;
                 
                 case '\r':
                 case '\n':
                     cmd[i] = '\0';
                     i = 0;
                     cli_state = CLI_STATE_EXEC;
                     break;
                 
                 default:
                     if(i >= MAX_STR_LEN) {
                         break;
                     }
                     cmd[i++] = ch;
                     CLI_PRINT("%c", ch); 
             } 
             break;
             
        case CLI_STATE_EXEC:
             CLI_PRINT("\r\n");
             cli_exec(cmd);
             cli_state = CLI_STATE_INPUT;
             CLI_PRINT(CLI_PROMPT);
             break;
        
        default:
            break;
    }
}
