#include "../include/nexus.h"

int builtin_cd(char **args) {
        if (args[1] == NULL) {
                chdir(getenv("HOME"));
        } else {
                if (chdir(args[1]) != 0) {
                        perror("nexus: cd");
                }
        }
        return 1;
}

int builtin_pwd(char **args) {
        (void)args;
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
        }
        return 1;
}

int builtin_clear(char **args) {
        (void)args;
        printf("\033[H\033[J");
        return 1;
}

int builtin_history(char **args) {
        (void)args;
        for (int j = 0; j < history_count; j++) {
                printf("%d  %s\n", j + 1, history[j]);
        }
        return 1;
}

int builtin_exit(char **args) {
        (void)args;
        return 0;
}

int is_builtin(char *cmd) {
        char *builtins[] = {"cd", "pwd", "clear", "history", "exit", NULL};
        for (int i = 0; builtins[i] != NULL; i++) {
                if (strcmp(cmd, builtins[i]) == 0) return 1;
        }
        return 0;
}

int run_builtin(char **args) {
        if (strcmp(args[0], "cd") == 0)      return builtin_cd(args);
        if (strcmp(args[0], "pwd") == 0)     return builtin_pwd(args);
        if (strcmp(args[0], "clear") == 0)   return builtin_clear(args);
        if (strcmp(args[0], "history") == 0) return builtin_history(args);
        if (strcmp(args[0], "exit") == 0)    return builtin_exit(args);
        return 1;
}
