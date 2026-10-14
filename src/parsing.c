#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include "logic.h"
#include "commands.h"

char * peek(const char *s, int c);
void skip_spaces(char **cmd);

int 
get_prompt(char *buf, size_t nbuf) 
{
    write(2, "$ ", 2);
    memset(buf, 0, nbuf);
    fgets(buf,nbuf,stdin);
    if (buf[0] == 0) {return -1;}

    return 0;
}

struct cmd *
parse_exec_cmd(struct cmd *cmd, char *s) 
{
    struct exec_cmd *execcmd;
    execcmd = (struct exec_cmd *)cmd;
    int i;
    char *s_stop = s + strlen(s);
    char *token;

    for (i = 0; s < s_stop; i++)
    {
        if (*(token = get_token(&s)) != 0)
        {
            execcmd->args[i] = token;    
        }
    }
    if (i > MAXARGS) {
        shell_exit(1, "too many args!");
    }

    execcmd->args[i+1] = NULL;

    return cmd;
}

void
parse_semicolon_cmd(struct cmd *cmd, char *s)
{
    struct cmd *next_cmd;
    struct semicolon_cmd *semcmd = (struct semicolon_cmd *)cmd;
    
    char *start = s;
    char *semicolon;

    if ((semicolon = peek(start, ';')))
    {
        next_cmd = create_semicolon_cmd();
        semcmd->command = start;
        *semicolon = '\0';
        start = semicolon + 1;
        skip_spaces(&start);
        parse_semicolon_cmd(next_cmd, start);
        semcmd->next_command = (struct semicolon_cmd *)next_cmd;
    }
    else  
    {
        semcmd->command = start;
        semcmd->next_command = NULL; 
    }
}

void
parse_pipeline_cmd(struct cmd *cmd, char *s)
{
    struct pipeline_cmd *pipecmd = (struct pipeline_cmd *)cmd;
    char *left, *right;
    char *pipe = peek(s, '|');
    left = s;
    right = pipe + 1;
    *pipe = '\0';

    pipecmd->left = left;
    pipecmd->right = right;
}

char *
peek(const char *s, int c)
{
    char *need_c;
    if ((need_c = strchr(s,c)) != NULL && *(need_c - 1) != '\\')
    {
        return need_c;
    }
    return NULL;
}

int
check_built_in_commands(char *cmd)
{
    if (cmd[0] == 'e' && cmd[1] == 'x' && cmd[2] == 'i' && cmd[3] == 't' && (cmd[4] == ' ' || cmd[4] == '\n' || cmd[4] == '\0')) 
    {
        shell_exit(0,"");
        return 1;
    }

    else if (cmd[0] == 'c' && cmd[1] == 'd' && (cmd[2] == ' ' || cmd[2] == '\n'))
    {
        cmd += 2;
        skip_spaces(&cmd);
        char *path = get_token(&cmd);
        shell_cd(path);
        return 1;
    }

    else if (cmd[0] == 'p' && cmd[1] == 'w' && cmd[2] == 'd' && (cmd[3] == ' ' || cmd[3] == '\n' || cmd[3] == '\0'))
    {
        shell_pwd();
        return 1;
    }

    return 0;
}

void 
skip_spaces(char **cmd)
{
    while (isspace(**cmd))
        {
            (*cmd)++;
        }
}

struct cmd *
parse_command(char *buf)
{
    struct cmd *cmd;
    skip_spaces(&buf);


        if (peek(buf, ';'))
        {
            cmd = create_semicolon_cmd();
            parse_semicolon_cmd(cmd, buf);
        }

        else if (peek(buf, '|'))
        {
            cmd = create_pipeline_cmd();
        }

        else if (check_built_in_commands(buf))
        {
            return BUILT_IN; 
        }

        else 
        {
            cmd = create_exec_cmd();
            parse_exec_cmd(cmd, buf);
        }

    return cmd;
}