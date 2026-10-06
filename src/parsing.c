#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include "logic.h"
#include "commands.h"

int 
get_prompt(char *buf, size_t nbuf) 
{
    write(2, "$ ", 2);
    memset(buf, 0, nbuf);
    fgets(buf,nbuf,stdin);
    if (buf[0] == 0) {return -1;}

    return 0;
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
    if (i > MAXARGS) {
        shell_exit(1, "too many args!");
    }

    execcmd->args[i+1] = NULL;

    return cmd;
}

char *
peek(const char *s, int c)
{
    char *need_c;
    if ((need_c = strchr(s,c)) != NULL && *(need_c - 1) != '\\')
    {
        return need_c;
    }
    return NULL;
}