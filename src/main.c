#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "parsing.h"
#include "logic.h"
#include "commands.h"
#include "config.h"

void shell_exec(struct cmd *cmd) __attribute__((noreturn));
void shell_semicolon(struct cmd *cmd);
void shell_pipeline(struct cmd *cmd);
void shell_pipe_child(int fd[2], int ofd[2]);
void shell_redirection(struct cmd *cmd);
void shell_redir(const char *target, int old_fd);

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
            
            case REDIR:
                shell_redirection(cmd);
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

void 
shell_semicolon(struct cmd *cmd)
{
    struct chain_cmd *semcmd = (struct chain_cmd *)cmd;

    for ( ; semcmd; semcmd = semcmd->next_command)
    {
        printf("SEM CMD: %s\n", semcmd->command);
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
    int i = 0;
    int *ofd = fd[0];
    struct cmd *ecmd;

    if (shell_fork() == 0)
    {
        shell_pipe_child(fd[0], NULL);
        if ((ecmd = parse_command(pipecmd->command)) != BUILT_IN)
        {
            shell_exec(ecmd);
        }
        shell_exit(0, NULL);
    }
    pipecmd = pipecmd->next_command;

    for (; pipecmd->next_command; i++, pipecmd = pipecmd->next_command)
    {
        fd[i + 1] = malloc(sizeof(int) * 2);
        ofd = fd[i+1];
        shell_pipe(ofd);

        if (shell_fork() == 0)
        {
            shell_pipe_child(ofd, fd[i]);
            if ((ecmd = parse_command(pipecmd->command)) != BUILT_IN)
            {
                shell_exec(ecmd);
            }
            shell_exit(0, NULL);
        }
        close(fd[i][0]);
        close(fd[i][1]);
        free(fd[i]);
    }

    if (shell_fork() == 0)
    {
        shell_pipe_child(NULL, ofd);
        if ((ecmd = parse_command(pipecmd->command)) != BUILT_IN)
        {
            shell_exec(ecmd);
        }
        shell_exit(0, NULL);
    }
    close(ofd[0]);
    close(ofd[1]);
    free(ofd);
    free(fd);

    for (i += 2; i; i--)
    {
        wait(0);
    }
}

void 
shell_redirection(struct cmd *cmd)
{
    struct cmd *ecmd;

    struct redir_cmd *redcmd;
    redcmd = (struct redir_cmd *)cmd;

    switch (redcmd->fd) 
    {
        case 0:
            if (shell_fork() == 0)
            {
                shell_redir(redcmd->dest, 0);
                if ((ecmd = parse_command(redcmd->command)) != BUILT_IN)
                {
                    shell_exec(ecmd);
                }
                shell_exit(0, NULL);
            }
            break;

        case 1:
            if (shell_fork() == 0)
            {
                shell_redir(redcmd->dest, 1);
                if ((ecmd = parse_command(redcmd->command)) != BUILT_IN)
                {
                    shell_exec(ecmd);
                }
                shell_exit(0, NULL);
            }
    }
    wait(0);
}

void
shell_redir(const char *target, int old_fd)
{
    int new_fd; 
    if (old_fd)
    {
        new_fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }
    else  
    {
        new_fd = open(target, O_RDONLY);
    }

    dup2(new_fd, old_fd);
    close(new_fd); 
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

int 
main(void)
{
    char buf[PROMPT_BUFF];
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