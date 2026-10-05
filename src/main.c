#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
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

    static char pwd_buf[100];
    static size_t pwd_nbuf = sizeof(pwd_buf);

    while (get_prompt(buf, nbuf) >= 0)
    {
        memset(pwd_buf, 0, pwd_nbuf);
        char *cmd = buf;
        static int ret;

        while (*cmd == ' ' || *cmd == '\t')
        {
            cmd++;
        }

        if (*cmd == '\n')
        {
            continue;
        }

        if (cmd[0] == 'e' && cmd[1] == 'x' && cmd[2] == 'i' && cmd[3] == 't' && (cmd[4] == ' ' || cmd[4] == '\n')) 
        {
            exit(0);
        }

        else if (cmd[0] == 'c' && cmd[1] == 'd' && (cmd[2] == ' ' || cmd[2] == '\n'))
        {
            cmd[strlen(cmd) - 1] = 0;
            ret = chdir(cmd + 3);
            if (ret != 0)
            {
                perror("chdir!");
            }
        }

        else if (cmd[0] == 'p' && cmd[1] == 'w' && cmd[2] == 'd' && (cmd[3] == ' ' || cmd[3] == '\n'))
        {
            getcwd(pwd_buf, pwd_nbuf);
            printf("%s\n", pwd_buf);
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