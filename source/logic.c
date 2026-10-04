#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
panic(const char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}


struct cmd *
create_exec_cmd(void)
{
    struct exec_cmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = EXEC
    return (struct cmd *)cmd
}