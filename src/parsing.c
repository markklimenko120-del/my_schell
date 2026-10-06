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

int
check_built_in_commands(char *cmd)
{
    if (cmd[0] == 'e' && cmd[1] == 'x' && cmd[2] == 'i' && cmd[3] == 't' && (cmd[4] == ' ' || cmd[4] == '\n')) 
    {
        shell_exit(0,"");
        return 1;
    }

    else if (cmd[0] == 'c' && cmd[1] == 'd' && (cmd[2] == ' ' || cmd[2] == '\n'))
    {
        shell_cd(cmd);
        return 1;
    }

    else if (cmd[0] == 'p' && cmd[1] == 'w' && cmd[2] == 'd' && (cmd[3] == ' ' || cmd[3] == '\n'))
    {
        shell_pwd();
        return 1;
    }

    return 0;
}

void 
skip_spaces(char **cmd)
{
    while (**cmd == ' ' || **cmd == '\t')
        {
            (*cmd)++;
        }
}