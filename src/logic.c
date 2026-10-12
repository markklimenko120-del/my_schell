#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "logic.h"
#include "commands.h"
#include "parsing.h"

int 
shell_fork(void)
{   
    int pid;

    if ((pid = fork()) == -1) 
    {
        shell_exit(1, "fork!");
    }

    return pid;
}


char *
get_token(char **start_s)
{
    char *s;
    s = *start_s;
    char *ret;
    
    while(*s)
    {
        if (!isspace(*s))
        {
            s++;
        }
        else 
        {
            *s = '\0';
            ret = malloc(s - *start_s);
            memcpy(ret, *start_s, s - *start_s);
            *start_s = s + 1;
            return ret;
        }
    }
    return NULL;
}

struct cmd *
create_exec_cmd(void) 
{
    struct exec_cmd *cmd;
    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = EXEC;

    return (struct cmd *)cmd;
}

struct cmd *
create_semicolon_cmd(void)
{
    struct semicolon_cmd *cmd;
    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = SEMICOLON;

    return (struct cmd *)cmd;
}