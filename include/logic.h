#ifndef LOGIC_H
#define LOGIC_H

enum commands {
    BUILT_IN = 0,
    EXEC = 1,
    SEMICOLON = 2,
};

enum { MAXARGS = 10};
struct cmd 
{
    int type;
};

struct exec_cmd
{
    int type;
    char *args[MAXARGS];
};

struct semicolon_cmd
{
    int type;
    struct exec_cmd *execcmd;
    struct semicolo_cmd *next_command;
};

int shell_fork(void);
struct cmd * create_exec_cmd(void);
struct cmd * create_semicolon_cmd(void);
char * get_token(char **start_s);

#endif