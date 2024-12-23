#ifndef CORE_H
#define CORE_H
#include <stdlib.h>

#define MAXARGS 128

/*
 * Evaluate the command line that the user has just typed in
 *
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.
 */
void eval(char *cmdline);
//
// /*
//  * If the user has typed a built-in command then execute
//  *    it immediately.
//  */
// int builtin_cmd(char **argv);
//
// void run_command(char **argv, char *cmdline, int bg);
//
// void do_bg(char **argv);
//
// void do_fg(char **argv);
//
// void waitfg(pid_t pid);
#endif
