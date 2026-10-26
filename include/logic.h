#ifndef LOGIC_H
#define LOGIC_H

enum commands {
    BUILT_IN = 0,
    EXEC = 1,
    SEMICOLON = 2,
    PIPE = 3,
    REDIR = 4,
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

struct chain_cmd /*List and Pipeline*/
{
    int type;
    char *command;
    int count_command;
    struct chain_cmd *next_command;
};

struct redir_cmd 
{
    int type;
    char *command;
    char *dest;
    int fd;
}

int shell_fork(void);
void shell_pipe(int *fd);
struct cmd * create_exec_cmd(void);
struct cmd * create_chain_cmd(int type); 
char * get_token(char **start_s);

#endif