#include <stdio.h>
#include "config_pars.h"

#ifndef PARS_H
#define PARS_H
int get_prompt(char *buf, size_t nbuf);
struct cmd * parse_command(char *buf, config *cfg);
struct cmd * parse_exec_cmd(struct cmd *cmd, char *s);
struct cmd * parse_chain_cmd(struct cmd *cmd, char *s);
char * peek(const char *s, int c);
int check_built_in_commands(char *cmd, config *cfg);
void skip_spaces(char **cmd);
#endif