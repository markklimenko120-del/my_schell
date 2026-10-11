#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "parsing.h"
#include "logic.h"
#include "commands.h"

void shell_exec(struct cmd *cmd, char *buf);

void
runcmd(char *buf)
{
    struct cmd *cmd;
    cmd = parse_command(buf);
    if (cmd == BUILT_IN)
    {
        return; /*It's built-in command*/
    }

    switch (cmd->type)
    {
        default:
            shell_exit(1, "runcmd!");
            break;

        case EXEC:
            if (shell_fork() == 0)
            {
                shell_exec(cmd, buf);
            }
            wait(0);
            break;
    }

    return;
}

void 
shell_exec(struct cmd *cmd, char *s)
{
    struct exec_cmd *execcmd;

    cmd = parse_exec_cmd(cmd, s);
    execcmd = (struct exec_cmd *)cmd;
    if (execcmd->args[0] == 0)
    {
        shell_exit(1, "non argumets!");
        return;
    }
    execv(execcmd->args[0], execcmd->args);
    free(cmd);
    shell_exit(0, "");
}

int 
main(void)
{
    char buf[100];
    size_t nbuf = sizeof(buf);

    while (get_prompt(buf, nbuf) >= 0)
    {
        char *cmd = buf;

        skip_spaces(&cmd);

        if (*cmd == '\n')
        {
            continue;
        }

        else if (*cmd != 0)
        {
            runcmd(cmd);
        }
    }
    return 0;
}