#ifndef COMM_H
#define COMM_H
void shell_cd(char *s);
void shell_exit(int n, const char *error) __attribute__((noreturn));
void shell_pwd(void);
#endif