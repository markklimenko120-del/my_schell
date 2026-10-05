#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parsing.h"
#include "logic.h"

void
runcmd(char *buf)
{
    struct cmd *cmd;
    cmd = create_exec_cmd();

    struct exec_cmd *execcmd;

    switch (cmd->type)
    {
        default:
            panic("runcmd!");
            break;
        
        case EXEC:
            cmd = parse_exec_cmd(cmd, buf);
            execcmd = (struct exec_cmd *)cmd;

            if (execcmd->args[0] == 0)
            {
                panic("non argumets!");
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
    static char buf[100];
    static size_t nbuf = sizeof(buf);

    while (get_prompt(buf, nbuf) >= 0)
    {
        char *cmd = buf;

        while (*cmd == ' ' || *cmd == '\t')
        {
            cmd++;
        }

        if (*cmd == '\n')
        {
            continue;
        }

        if (cmd[0] == 'e' && cmd[1] == 'x' && cmd[2] == 'i' && cmd[3] == 't') 
        {
            exit(0);
        }
        else if (buf[0] != 0)
        {
            if (shell_fork() == 0)
            {
                runcmd(buf);
            }
            wait(0);
        } else {
            continue;
        }
    }
    return 0;
}