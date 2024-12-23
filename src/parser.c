#include "parser.h"
#include "job.h"
#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parseline(const char *cmdline, char **argv) {
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf) - 1] = ' ';   /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
        buf++;
        delim = strchr(buf, '\'');
    } else {
        delim = strchr(buf, ' ');
    }

    while (delim) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* ignore spaces */
            buf++;

        if (*buf == '\'') {
            buf++;
            delim = strchr(buf, '\'');
        } else {
            delim = strchr(buf, ' ');
        }
    }
    argv[argc] = NULL;

    if (argc == 0) /* ignore blank line */
        return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc - 1] == '&')) != 0) {
        argv[--argc] = NULL;
    }
    return bg;
}

struct job_t *parse_job_index(char **argv) {
    if (argv[1] == NULL) {
        printf("%s command requires PID or %cjobid argument\n", argv[0], '%');
        return NULL;
    }
    char *index = argv[1];

    if (!valid_id(index)) {
        printf("%s: argument must be a PID or %cjobid\n", argv[0], '%');
        return NULL;
    }

    struct job_t *job;
    pid_t pid = -1;
    int jid = -1;

    if (index[0] != '%') {
        pid = atoi(index);
        job = getjobpid(jobs, pid);
    } else {
        jid = atoi(index + 1);
        job = getjobjid(jobs, jid);
    }

    if (job != NULL) {
        return job;
    }

    if (jid != -1) {
        printf("%c%d: No such job\n", '%', jid);
    } else {
        printf("(%d): No such process\n", pid);
    }

    return NULL;
}
