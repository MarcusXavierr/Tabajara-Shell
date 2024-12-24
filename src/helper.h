#ifndef HELPER_H
#define HELPER_H
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

typedef void handler_t(int);

/* unix-style error routine */
void unix_error(char *msg);

/* application-style error routine */
void app_error(char *msg);

/* print a help message */
void usage(void);

/* wrapper for the sigaction function */
handler_t *Signal(int signum, handler_t *handler);

/* Just a fork wrapper that handles syscall error */
pid_t Fork(void);

/* Just a waitpid wrapper that handles syscall error */
pid_t Waitpid(pid_t pid, int *iptr, int options);

/* Just an execve wrapper that handles syscall error */
void Execve(const char *filename, char *const argv[], char *const envp[]);

// TODO: add comment
int valid_id(char *id);

char **parse_path(int *counter, char* path);

char *find_valid_program_path(char **paths, int num_paths, char *program);
#endif
