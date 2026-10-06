#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "parsing.h"
#include "logic.h"
#include "commands.h"

void
runcmd(char *buf)
{
    struct cmd *cmd;
    cmd = create_exec_cmd();

    struct exec_cmd *execcmd;

    switch (cmd->type)
    {
        default:
            shell_exit(1, "runcmd!");
            break;
        
        case EXEC:
            cmd = parse_exec_cmd(cmd, buf);
            execcmd = (struct exec_cmd *)cmd;

            if (execcmd->args[0] == 0)
            {
                shell_exit(1, "non argumets!");
                break;
            }
            execv(execcmd->args[0], execcmd->args);
            break;
    }

    exit(0);
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