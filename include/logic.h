#ifndef LOGIC_H
#define LOGIC_H

enum commands {
    BUILT_IN = 0,
    EXEC = 1,
    SEMICOLON = 2,
    PIPE = 3,
};

enum { MAXARGS = 10 };
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
    char *command;
    struct semicolon_cmd *next_command;
};

struct pipeline_cmd
{
    int type;
    char *left;
    char *right;
};

int shell_fork(void);
struct cmd * create_exec_cmd(void);
struct cmd * create_semicolon_cmd(void);
struct cmd * create_pipeline_cmd(void);
char * get_token(char **start_s);

#endif