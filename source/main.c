#include <stdlib.h>
#include <stdio.h>
#include "parsing.h"

int 
main(void)
{
    static char buf[100];
    static size_t nbuf = sizeof(buf);

    while (get_prompt(buf, nbuf) >= 0)
    {
        char *cmd = buf;

        while (*cmd == ' ' || *cmd == '\t')
        {
            cmd++;
        }

        if (*cmd == '\n')
        {
            continue;
        }

        if (cmd[0] == 'e' && cmd[1] == 'x' && cmd[2] == 'i' && cmd[3] == 't') 
        {
            exit(0);
        }
    }
    return 0;
}