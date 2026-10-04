#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
#include <string.h>
=======
#include "logic.h"
>>>>>>> ec5b5e4 (Add include logic.h in source/logic.c)

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