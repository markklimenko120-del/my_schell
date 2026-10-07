#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "parsing.h"
#include "logic.h"
#include "commands.h"

void shell_exec(struct cmd *cmd, char *s);

void
runcmd(char *buf)
{
    struct cmd *cmd;
    cmd = parse_command(buf);

    switch (cmd->type)
    {
        default:
            shell_exit(1, "runcmd!");
            break;
        
        case EXEC:
            shell_exec(cmd, buf);
            break;
    }

    exit(0);
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
    }
    execv(execcmd->args[0], execcmd->args);
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

        else if (check_built_in_commands(cmd))
        {
            continue;
        }

        else if (*cmd != 0)
        {

            if (shell_fork() == 0)
            {
                runcmd(cmd);
            }
            wait(0);
        }
    }
    return 0;
}