#ifndef LOGIC_H
#define LOGIC_H

#define EXEC 1
#define MAXARGS 10

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
    struct semicolo_cmd *next_command;
};

int shell_fork(void);
struct cmd * create_exec_cmd(void);
char * get_token(char **start_s);

#endif