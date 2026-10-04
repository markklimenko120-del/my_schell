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

<<<<<<< HEAD
void panic(char *s);
struct cmd * create_exec_cmd(void);
=======
void panic(const char *s);
>>>>>>> ec5b5e4 (Add include logic.h in source/logic.c)

#endif