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
void shell_pipe_child(int fd[2], int ofd[2]);

void
runcmd(char *s)
{
    struct cmd *cmd;
    pid_t cpid;
    int status;

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
                if ((cpid = shell_fork()) == 0)
                {
                    shell_exec(cmd);
                }
                waitpid(cpid, &status, 0);
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

#ifdef OLD_PIPELINE
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

    shell_pipe(fd);
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
#else
void 
shell_semicolon(struct cmd *cmd)
{
    struct chain_cmd *semcmd = (struct chain_cmd *)cmd;

    for ( ; semcmd; semcmd = semcmd->next_command)
    {
        runcmd(semcmd->command);
    }
}

void
shell_pipeline(struct cmd *cmd)
{
    struct chain_cmd *pipecmd = (struct chain_cmd *)cmd;
    int **fd = malloc(pipecmd->count_command * sizeof(int *));
    fd[0] = malloc(sizeof(int) * 2);
    shell_pipe(fd[0]);
    // int i;
    struct cmd *ecmd;

    if (shell_fork() == 0)
    {
        shell_pipe_child(fd[0], NULL);
        if ((ecmd = parse_command(pipecmd->command)) != BUILT_IN)
        {
            shell_exec(ecmd);
        }
    }
    pipecmd = pipecmd->next_command;

    if (shell_fork() == 0)
    {
        shell_pipe_child(NULL, fd[0]);
        if ((ecmd = parse_command(pipecmd->command)) != BUILT_IN)
        {
            shell_exec(ecmd);
        }
    }

    close(fd[0][0]);
    close(fd[0][1]);
    free(fd[0]);
    free(fd);

    wait(0);
    wait(0);
}

void
shell_pipe_child(int fd[2], int ofd[2])
{
    if (fd)
    {
        if (fd[1] != 1)
        {
            dup2(fd[1], 1);
        }

        close(fd[0]);
        close(fd[1]);
    }
    if (ofd)
    {
        if (ofd[0] != 0)
        {
            dup2(ofd[0], 0); 
        }

        if (fd != ofd)
        {
            close(ofd[1]);
            close(ofd[0]);
        }
    }
}
#endif

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