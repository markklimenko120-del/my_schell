#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"

void
shell_cd(char *s)
{
    int ret;
    ret = chdir(s);
    if (ret != 0)
    {
        perror("chdir!");
    }
}

void
shell_exit(int n, const char *error) 
{
    if (!n)
    {
        exit(0);
    }
    perror(error);
    exit(n);
}

void 
shell_pwd(void)
{
    char buf[PWD_BUFF];
    size_t nbuf = sizeof(buf);
    memset(buf, 0, nbuf);
    getcwd(buf, nbuf);
    printf("%s\n", buf);
}