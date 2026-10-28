#include <endian.h>
#include <stdio.h>
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

void
shell_pipe(int *fd)
{
    if (pipe(fd) < 0)
    {
        shell_exit(1, "pipe");
    }
}

char *
get_token(char **start_s)
{
    char *s;
    s = *start_s;
    char *ret;

    for (; !isspace(*s) && *s != '\0'; s++) {}

    *s = '\0';
    ret = *start_s;
    if (*ret == '\0')
    {
        ret++;
    }
    *start_s = s + 1;
    return ret;
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
create_chain_cmd(int type)
{
    struct chain_cmd *cmd;
    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = type;

    return (struct cmd *)cmd;
}

struct cmd *
create_redir_cmd(void)
{
    struct redir_cmd *cmd;
    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = REDIR;

    return (struct cmd *)cmd;
}