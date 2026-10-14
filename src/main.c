#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "parsing.h"
#include "logic.h"
#include "commands.h"

void shell_exec(struct cmd *cmd) __attribute__((noreturn));
void shell_semicolon(struct cmd *cmd);
void shell_pipeline(struct cmd *cmd);

void
runcmd(char *s)
{
    struct cmd *cmd;

    if ((cmd = parse_command(s)) == BUILT_IN)
    {
        return;
    }

    else
    {
        switch (cmd->type)
        {
            case SEMICOLON:
                shell_semicolon(cmd);
                break;
        
            case PIPE:
                shell_pipeline(cmd);
                break;

            case EXEC:
                if (shell_fork() == 0)
                {
                    shell_exec(cmd);
                }
                wait(0);
                break;
        }
    }
    free(cmd);
}

void 
shell_exec(struct cmd *cmd)
{
    struct exec_cmd *execcmd;

    execcmd = (struct exec_cmd *)cmd;
    if (execcmd->args[0] == 0 && execcmd->args[0][0] != '\0')
    {
        shell_exit(1, "exec");
    }
    execvp(execcmd->args[0], execcmd->args);
    shell_exit(0, "");
}

void 
shell_semicolon(struct cmd *cmd)
{
    struct semicolon_cmd *semcmd = (struct semicolon_cmd *)cmd;

    for ( ; semcmd; semcmd = semcmd->next_command)
    {
        runcmd(semcmd->command);
    }
}

void
shell_pipeline(struct cmd *cmd)
{
    struct pipeline_cmd *pipecmd = (struct pipeline_cmd *)cmd;
    int fd[2];

    if (pipe(fd) < 0)
    {
        shell_exit(1, "pipe");
    }

    if (shell_fork() == 0)
    {
        struct cmd *cmdl;
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);

        if ((cmdl = parse_command(pipecmd->left)))
        {
            shell_exec(cmdl);
        }
    }

    if (shell_fork() == 0)
    {
        struct cmd *cmdr;
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);

        if ((cmdr = parse_command(pipecmd->right)))
        {
            shell_exec(cmdr);
        }
    }

    close(fd[0]);
    close(fd[1]);

    wait(0);
    wait(0);
}

int 
main(void)
{
    char buf[100];
    size_t nbuf = sizeof(buf);

    while (get_prompt(buf, nbuf) >= 0)
    {
        char *c = buf;
 
        if (*c == '\n')
        {
            continue;
        }

        else if (*c != 0)
        {
            runcmd(c);
        }
    }
    return 0;
}