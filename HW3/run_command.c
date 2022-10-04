/*
 * run_command.c :    do the fork, exec stuff, call other functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include "shell.h"

void run_command(char **myArgv) {
    pid_t pid;
    int stat, cur = 0;

    /* Create a new child process.
     * Fill in code.
	 */
    pid = fork();

    switch (pid) {

        /* Error. */
        case -1 :
            perror("fork");
            exit(errno);

        /* Parent. */
        default :
            /* Wait for child to terminate.
             * Fill in code.
			 */

            if(is_background(myArgv) == FALSE){
                if(waitpid(-1, &stat, 0) == -1){
                    perror("waitpid");
                    exit(1);
                }
            }

            /* Optional: display exit status.  (See wstat(5).)
             * Fill in code.
			 */
            if(WIFSIGNALED(stat)) printf("terminated by signal %d\n", WTERMSIG(stat));
            else if(WIFEXITED(stat)) printf("exited with status %d\n", WEXITSTATUS(stat));
            else if(WIFSTOPPED(stat)) printf("stopped by signal %d\n", WSTOPSIG(stat));
            return;

        /* Child. */
        case 0 :
            /* Run command in child process.
             * Fill in code.
			 */
			if(is_background(myArgv) == TRUE){
				while(strcmp(myArgv[cur], "&") != 0) cur += 1;
				myArgv[cur] = (char *) NULL;
			}
            execvp(myArgv[0], myArgv);

            /* Handle error return from exec */
			exit(errno);
    }
}
