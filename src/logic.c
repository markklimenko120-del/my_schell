#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "logic.h"


void
panic(const char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}

int 
shell_fork(void)
{   
    int pid;

    if ((pid = fork()) == -1) 
    {
        panic("fork!");
    }

    return pid;
}


char *
get_token(char **start_s)
{
    static char *s;
    s = *start_s;
    static char *ret;
    
    for (;;)
    {
        if (*s != ' ' && *s != '\t' && *s != 0 && *s != '\n')
        {
            s++;
        } else {
            ret = malloc(s - *start_s);
            memcpy(ret, *start_s, s - *start_s);
            *start_s = s + 1;
            return ret;
        }
    }
}

struct cmd *
parse_exec_cmd(struct cmd *cmd, char *s) 
{
    struct exec_cmd *execcmd;
    execcmd = (struct exec_cmd *)cmd;
    int i;

    for (i = 0; *s != 0; i++)
    {
        execcmd->args[i] = get_token(&s);    
    }
    execcmd->args[i+1] = NULL;

    return cmd;
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