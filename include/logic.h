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


void panic(const char *s);
int shell_fork(void);
struct cmd * create_exec_cmd(void);
char * get_token(char **start_s);
struct cmd * parse_exec_cmd(struct cmd *cmd, char *s);

#endif