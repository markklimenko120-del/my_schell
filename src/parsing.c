#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include "logic.h"
#include "commands.h"

char * peek(const char *s, int c);

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
    char *token;

    for (i = 0; *s != 0; i++)
    {
        if ((token = get_token(&s)))
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
        *semicolon = 0;
        start = semicolon + 1;
        parse_semicolon_cmd(next_cmd, start);
        semcmd->next_command = (struct semicolon_cmd *)next_cmd;
    }
    else  
    {
        semcmd->command = start;
        semcmd->next_command = NULL; 
    }
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
        if (check_built_in_commands(buf))
        {
            return BUILT_IN; 
        }

        else if (peek(buf, ';'))
        {
            cmd = create_semicolon_cmd();
        }
        
        else 
        {
            cmd = create_exec_cmd();
        }

    return cmd;
}