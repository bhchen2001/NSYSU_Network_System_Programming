/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2022.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Supplementary program for Chapter 22 */

/* sig_speed_sigsuspend.c

   This program times how fast signals are sent and received.

   The program forks to create a parent and a child process that alternately
   send signals to each other (the child starts first). Each process catches
   the signal with a handler, and waits for signals to arrive using
   sigsuspend().

   Usage: $ time ./sig_speed_sigsuspend num-sigs

   The 'num-sigs' argument specifies how many times the parent and
   child send signals to each other.

   Child                                  Parent

   for (s = 0; s < numSigs; s++) {        for (s = 0; s < numSigs; s++) {
       send signal to parent                  wait for signal from child
       wait for a signal from parent          send a signal to child
   }                                      }
*/
#include <signal.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>
#include "tlpi_hdr.h"

/* Print a diagnostic message that contains a function name ('fname'),
   the value of a command-line argument ('arg'), the name of that
   command-line argument ('name'), and a diagnostic error message ('msg'). */
static void
gnFail(const char *fname, const char *msg, const char *arg, const char *name)
{
    fprintf(stderr, "%s error", fname);
    if (name != NULL)
        fprintf(stderr, " (in %s)", name);
    fprintf(stderr, ": %s\n", msg);
    if (arg != NULL && *arg != '\0')
        fprintf(stderr, "        offending text: %s\n", arg);

    exit(EXIT_FAILURE);
}

/* Convert a numeric command-line argument ('arg') into a long integer,
   returned as the function result. 'flags' is a bit mask of flags controlling
   how the conversion is done and what diagnostic checks are performed on the
   numeric result; see get_num.h for details.

   'fname' is the name of our caller, and 'name' is the name associated with
   the command-line argument 'arg'. 'fname' and 'name' are used to print a
   diagnostic message in case an error is detected when processing 'arg'. */

static long
getNum(const char *fname, const char *arg, int flags, const char *name)
{
    long res;
    char *endptr;
    int base;

    if (arg == NULL || *arg == '\0')
        gnFail(fname, "null or empty string", arg, name);

    base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 :
                        (flags & GN_BASE_16) ? 16 : 10;

    errno = 0;
    res = strtol(arg, &endptr, base);
    if (errno != 0)
        gnFail(fname, "strtol() failed", arg, name);

    if (*endptr != '\0')
        gnFail(fname, "nonnumeric characters", arg, name);

    if ((flags & GN_NONNEG) && res < 0)
        gnFail(fname, "negative value not allowed", arg, name);

    if ((flags & GN_GT_0) && res <= 0)
        gnFail(fname, "value must be > 0", arg, name);

    return res;
}

/* Convert a numeric command-line argument string to an integer. See the
   comments for getNum() for a description of the arguments to this function. */

int
getInt(const char *arg, int flags, const char *name)
{
    long res;

    res = getNum("getInt", arg, flags, name);

    if (res > INT_MAX || res < INT_MIN)
        gnFail("getInt", "integer out of range", arg, name);

    return res;
}

static void
handler(int sig)
{
}

#define TESTSIG SIGUSR1

int
main(int argc, char *argv[])
{
	//clock_t start = clock();
    struct timeval start, end;
	gettimeofday(&start, NULL);
	if (argc != 2 || strcmp(argv[1], "--help") == 0){
        // usageErr("%s num-sigs\n", argv[0]);
        perror("num-sigs");
        exit(1);
    }

    int numSigs = getInt(argv[1], GN_GT_0, "num-sigs");

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(TESTSIG, &sa, NULL) == -1){
        // errExit("sigaction");
        perror("sigaction");
        exit(1);
    }

    /* Block the signal before fork(), so that the child doesn't manage
       to send it to the parent before the parent is ready to catch it */

    sigset_t blockedMask, emptyMask;
    sigemptyset(&blockedMask);
    sigaddset(&blockedMask, TESTSIG);
    if (sigprocmask(SIG_SETMASK, &blockedMask, NULL) == -1){
        // errExit("sigprocmask");
        perror("sigprocmask");
        exit(1);
    }

    sigemptyset(&emptyMask);

    pid_t childPid = fork();
    switch (childPid) {
    case -1: 
        // errExit("fork");
        perror("fork");
        exit(1);

    case 0:     /* child */
        for (int scnt = 0; scnt < numSigs; scnt++) {
            if (kill(getppid(), TESTSIG) == -1){
                // errExit("kill");
                perror("kill");
                exit(1);
            }
            if (sigsuspend(&emptyMask) == -1 && errno != EINTR){
                    // errExit("sigsuspend");
                    perror("sigsuspend");
                    exit(1);
            }
        }
        exit(EXIT_SUCCESS);

    default: /* parent */
        for (int scnt = 0; scnt < numSigs; scnt++) {
            if (sigsuspend(&emptyMask) == -1 && errno != EINTR){
                    // errExit("sigsuspend");
                    perror("sigsuspend");
                    exit(1);
            }
            if (kill(childPid, TESTSIG) == -1){
                // errExit("kill");
                perror("kill");
                exit(1);
            }
        }
		gettimeofday(&end, NULL);
		printf("time interval: %d", end.tv_sec - start.tv_sec);
		// clock_t interval = clock() - start;
		// printf("time interval = %d", interval / CLOCKS_PER_SEC);
        exit(EXIT_SUCCESS);
    }
}
