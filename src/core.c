#include "core.h"
#include "helper.h"
#include "job.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ; /* defined in libc */

/*
 * If the user has typed a built-in command then execute
 *    it immediately.
 */
int builtin_cmd(char **argv);

void run_command(char **argv, char *cmdline, int bg);

void do_bg(char **argv);

void do_fg(char **argv);

void waitfg(pid_t pid);

int find_on_path(char **argv);

int do_cd(char **args);

void eval(char *cmdline) {
    char **argv = (char **)malloc(MAXARGS * sizeof(char *));
    for (int i = 0; i < MAXARGS; i++)
        argv[i] = (char *)malloc(100 * sizeof(char));

    int bg = parseline(cmdline, argv);

    if (argv == NULL) {
        app_error("wtf is happening");
        return;
    }

    if (builtin_cmd(argv) == 1) {
        return;
    }

    run_command(argv, cmdline, bg);
}

int builtin_cmd(char **argv) {
    if (strcmp(argv[0], "quit") == 0) {
        exit(0);
        return 1;
    }
    if (strcmp(argv[0], "jobs") == 0) {
        listjobs(jobs);
        return 1;
    }

    if (strcmp(argv[0], "bg") == 0) {
        do_bg(argv);
        return 1;
    }

    if (strcmp(argv[0], "fg") == 0) {
        do_fg(argv);
        return 1;
    }

    if (strcmp(argv[0], "cd") == 0) {
        do_cd(argv);
        return 1;
    }

    return 0; /* not a builtin command */
}

void run_command(char **argv, char *cmdline, int bg) {
    pid_t pid;
    if ((pid = Fork()) == 0) {
        // INFO: Set a different program group ID for the fork child
        find_on_path(argv);
        setpgid(0, 0);
        Execve(argv[0], argv, environ);
    } else {
        addjob(jobs, pid, bg ? BG : FG, cmdline);
        if (bg) {
            printf("[%d] (%d) %s\n", maxjid(jobs), pid, cmdline);
        } else {
            waitfg(pid);
        }
    }
}

/*
 * do_bg - Execute the builtin bg and fg commands
 */
void do_bg(char **argv) {
    struct job_t *job = parse_job_index(argv);
    if (job == NULL) {
        return;
    }

    killpg(job->pid, SIGCONT);
    job->state = BG;
    return;
}

/*
 * do_fg - Execute the builtin bg and fg commands
 */
void do_fg(char **argv) {
    struct job_t *job = parse_job_index(argv);
    if (job == NULL) {
        return;
    }

    killpg(job->pid, SIGCONT);
    job->state = FG;
    waitfg(job->pid);
    return;
}

/*
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid) {
    int child_status;
    Waitpid(pid, &child_status, WUNTRACED);

    if (WIFSTOPPED(child_status)) {
        struct job_t *job = getjobpid(jobs, pid);
        job->state = ST;
        printf("Job [%d] (%d) stopped by signal 20\n", pid2jid(pid), pid);
        return;
    } else if (WIFSIGNALED(child_status) && WTERMSIG(child_status) == SIGINT) {
        printf("Job [%d] (%d) terminated by signal 2\n", pid2jid(pid), pid);
    }

    if (verbose) {
        printf("Process (%d) no longer the fg process\n", pid);
    }

    deletejob(jobs, pid);
    return;
}

int do_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}

int find_on_path(char **argv) {
    char *path = getenv("PATH");
    int counter = 0;
    char *value = find_valid_program_path(parse_path(&counter, path), counter, argv[0]);
    if (value != NULL) {
        argv[0] = value;
        return 1;
    }

    return 0;
}
