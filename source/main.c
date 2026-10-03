#include <stdio.h>
#include "parsing.h"

int 
main(void)
{
    char buf[100];
    for (;;)
    {
        get_prompt(buf, sizeof(buf));
    }
    return 0;
}