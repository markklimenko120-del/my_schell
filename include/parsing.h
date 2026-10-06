#include <stdio.h>

#ifndef PARS_H
#define PARS_H
int get_prompt(char *buf, size_t nbuf);
struct cmd * parse_exec_cmd(struct cmd *cmd, char *s);
int peek(const char *s, int c);
#endif