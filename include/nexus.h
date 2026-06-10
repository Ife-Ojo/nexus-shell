#define _POSIX_C_SOURCE 200809L
#ifndef NEXUS_H
#define NEXUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define HISTORY_MAX 100
#define HISTORY_FILE "/.nexus_history"

extern char *history[];
extern int history_count;

/* history */
void save_history();
void load_history();

/* builtins */
int builtin_cd(char **args);
int builtin_pwd(char **args);
int builtin_clear(char **args);
int builtin_history(char **args);
int builtin_exit(char **args);
int is_builtin(char *cmd);
int run_builtin(char **args);

#endif

