#ifndef JOB_H
#define JOB_H
#include <sys/types.h>

#define MAXLINE 1024
#define MAXJOBS 16

/* undefined */
#define UNDEF 0
/* running in foreground */
#define FG 1
/* running in background */
#define BG 2
/* stopped */
#define ST 3

struct job_t {
    pid_t pid;
    int jid;   /* job ID [1, 2, ...] */
    int state; /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];
};

extern int verbose;
extern int nextjid;
extern struct job_t jobs[MAXJOBS]; /* The job list */

/* Clear the entries in a job struct */
void clearjob(struct job_t *job);

/* Initialize the job list */
void initjobs(struct job_t *jobs);

/* Returns largest allocated job ID */
int maxjid(struct job_t *jobs);

/* Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);

/* Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid);

/* Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);

/* Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid);

/* Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs);

/* Map process ID to job ID */
int pid2jid(pid_t pid);

/* Print the job list */
void listjobs(struct job_t *jobs);
#endif
