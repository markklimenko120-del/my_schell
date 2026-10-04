#include <stdio.h>
#include <stdlib.h>
#include "logic.h"

void
panic(const char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}