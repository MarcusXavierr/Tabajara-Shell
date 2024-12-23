#ifndef PARSER_H
#define PARSER_H

#define MAXLINE 1024

/*
 * Parse the command line and build the argv array.
 *
 * Characters enclosed in single quotes are treated as a single
 * argument.
 * Return true if the user has requested a BG job, false if
 * the user has requested a FG job.
 */
int parseline(const char *cmdline, char **argv);


struct job_t *parse_job_index(char **argv);
#endif
