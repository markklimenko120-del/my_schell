#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logic.h"

void
panic(const char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}



char *
get_token(char **start_s)
{
    static char *s;
    s = *start_s;
    static char *ret;

    
    for (;;)
    {
        if (*s != ' ' && *s != '\t' && *s != 0)
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
create_exec_cmd(void) 
{
    struct exec_cmd *cmd;
    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = EXEC;

    return (struct cmd *)cmd;
}