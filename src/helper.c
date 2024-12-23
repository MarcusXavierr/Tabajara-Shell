#include "helper.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
