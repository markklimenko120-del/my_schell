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
check_built_in_commads(char *cmd)
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

        else if (check_built_in_commads(cmd))
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