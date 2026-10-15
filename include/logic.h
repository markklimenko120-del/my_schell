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

#ifdef OLD_PIPELINE
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
#else

struct chain_cmd /*List and Pipeline*/
{
    int type;
    char *command;
    struct chain_cmd *next_command;
};
#endif

int shell_fork(void);
void shell_pipe(int *fd);
struct cmd * create_exec_cmd(void);
struct cmd * create_chain_cmd(int type); /*IF NOT OLF_PIPE*/
struct cmd * create_semicolon_cmd(void);
struct cmd * create_pipeline_cmd(void);
char * get_token(char **start_s);

#endif