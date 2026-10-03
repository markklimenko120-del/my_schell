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

#endif