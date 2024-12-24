#include "helper.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

extern char **environ;   /* defined in libc */

void unix_error(char *msg) {
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

void app_error(char *msg) {
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

void usage(void) {
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

handler_t *Signal(int signum, handler_t *handler) {
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");
    return (old_action.sa_handler);
}

pid_t Fork(void) {
    pid_t pid;

    if ((pid = fork()) < 0) {
        unix_error("Fork error");
    }

    return pid;
}

pid_t Waitpid(pid_t pid, int *iptr, int options) {
    pid_t retpid;

    if ((retpid = waitpid(pid, iptr, options)) < 0)
        unix_error("Waitpid error");
    return (retpid);
}

void Execve(const char *filename, char *const argv[], char *const envp[]) {
    if (execve(filename, argv, envp) < 0)
        unix_error("Execve error");
}

int valid_id(char *id) {
    int i = id[0] == '%' ? 1 : 0;
    int length = strlen(id);
    for (; i < length; i++) {
        if (!isdigit(id[i])) {
            return 0;
        }
    }

    return 1;
}

char **parse_path(int *counter, char* path) {
    if (path == NULL) {
        *counter = 0;
        return NULL;
    }

    *counter = 1;
    char *pointer = path;
    char *copy = calloc(sizeof(char), strlen(path) + 1);
    strcpy(copy, path);
    // Just counting how many separated items there are on the PATH
    while(*pointer != '\0') {
        if (*pointer++ == ':')
            (*counter)++;
    }

    char **tokens = malloc(sizeof(char*) * *counter);

    char delim[2] = ":";
    char *token = strtok(copy, delim);

    for (int i = 0; token != NULL && i < *counter; i++) {
        tokens[i] = token;
        token = strtok(NULL, ":");
    }

    return tokens;
}

void Stat(const char *filename, struct stat *buf)
{
    if (stat(filename, buf) < 0)
	unix_error("Stat error");
}

char *find_valid_program_path(char **paths, int num_paths, char *program) {
    for (int i = 0; i < num_paths; i++) {
        char *path = paths[i];
        char *buffer = calloc(sizeof(char), (strlen(path) + strlen(program) + 1));
        sprintf(buffer, "%s/%s", path, program);
        struct stat st;
        int status = stat(buffer, &st);
        if (status == 0 && st.st_mode & S_IXUSR) {
            return buffer;
        };
        free(buffer);
    }

    return NULL;
}
