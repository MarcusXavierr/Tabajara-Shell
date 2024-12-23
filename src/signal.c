#include "signal.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>
#include "job.h"
#include "helper.h"

void sigquit_handler(int sig) {
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}

void sigchld_handler(int sig) {
    int olderrno = errno;

    pid_t pid;
    while ((pid = wait3(NULL, WNOHANG, NULL)) > 0) {
        deletejob(jobs, pid);
    }

    errno = olderrno;
}

void sigint_handler(int sig) {
    pid_t pid = fgpid(jobs);
    if (pid > 0) {
        if (killpg(pid, SIGINT) < 0) {
            unix_error("kill (int) error");
        }

        // TODO: Find a way to create secure strings
        printf("Job [%d] (%d) terminated by signal 2\n", pid2jid(pid), pid);
        return;
    }

    char message[] = "\nGoodbye!";
    write(STDOUT_FILENO, message, sizeof(message));
    exit(0);
}

// TODO: Solucionar bug que a porcaria do sdriver não consegue executar nada após rodar o job após um sigstp
void sigtstp_handler(int sig) {
    int olderrno = errno;
    pid_t pid = fgpid(jobs);
    sigset_t mask_all, prev_mask;

    sigfillset(&mask_all);
    sigprocmask(SIG_BLOCK, &mask_all, &prev_mask);

    if (pid <= 0) {
        sigprocmask(SIG_SETMASK, &prev_mask, NULL);
        errno = olderrno;
        return;
    }

    struct job_t *job = getjobpid(jobs, pid);
    job->state = ST;
    sigprocmask(SIG_SETMASK, &prev_mask, NULL);
    if (killpg(pid, SIGTSTP) < 0) {
        unix_error("kill (tstp) error");
    }

    // TODO: Find a way to create secure strings
    printf("Job [%d] (%d) stopped by signal 20\n", pid2jid(pid), pid);
    errno = olderrno;
    return;
}
