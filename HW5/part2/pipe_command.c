/* 
 * pipe_command.c  :  deal with pipes
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "shell.h"

#define STD_OUTPUT 1
#define STD_INPUT  0

void pipe_and_exec(char **myArgv) {
  	int pipe_argv_index = pipe_present(myArgv);
  	int pipefds[2];
	char **left_argv;
	char **right_argv;
	pid_t pid;
	int i;

  	switch (pipe_argv_index) {

    	case -1:	/* Pipe at beginning or at end of argv;  See pipe_present(). */
      		fputs ("Missing command next to pipe in commandline.\n", stderr);
      		errno = EINVAL;	/* Note this is NOT shell exit. */
      		break;

    	case 0:	/* No pipe found in argv array or at end of argv array.
			See pipe_present().  Exec with whole given argv array. */
			execvp(myArgv[0], myArgv);
      		break;

    	default:	/* Pipe in the middle of argv array.  See pipe_present(). */

      		/* Split arg vector into two where the pipe symbol was found.
       		 * Terminate first half of vector.
			 *
       		 * Fill in code. */

			if((left_argv = (char **)malloc(pipe_argv_index * sizeof(char *))) == NULL){
				perror("malloc char **");
				exit(1);
			}

			for(i = 0; i < pipe_argv_index && myArgv[i] != (char *)NULL; i++){
				if((left_argv[i] = (char *)malloc(50 * sizeof(char *))) == NULL){
					perror("malloc char **");
					exit(1);
				}
				strcpy(left_argv[i], myArgv[i]);
			}
			left_argv[i] = '\0';

			if((right_argv = (char **)malloc(20 * sizeof(char *))) == NULL){
				perror("malloc char **");
				exit(1);
			}
			for(i = pipe_argv_index + 1; (myArgv[i] != (char *)NULL) && (strcmp(myArgv[i], "|")  != 0); i++){
				if((right_argv[i - pipe_argv_index - 1] = (char *)malloc(50 * sizeof(char *))) == NULL){
					perror("malloc char **");
					exit(1);
				}
				strcpy(right_argv[(i - pipe_argv_index - 1)], myArgv[i]);
			}
			right_argv[i - pipe_argv_index - 1] = '\0';

      		/* Create a pipe to bridge the left and right halves of the vector. 
			 *
			 * Fill in code. */

			if(pipe(pipefds) == -1){
				perror("pipe");
				exit(1);
			}

      		/* Create a new process for the right side of the pipe.
       		 * (The left side is the running "parent".)
       		 *
			 * Fill in code to replace the underline. */
      		switch(fork()) {

        		case -1 :
	  				break;

        		/* Talking parent.  Remember this is a child forked from shell. */
        		default :

	  				/* - Redirect output of "parent" through the pipe.
	  				 * - Don't need read side of pipe open.  Write side dup'ed to stdout.
	 	 			 * - Exec the left command.
					 *
					 * Fill in code. */

					close(pipefds[0]);
					if(pipefds[1] != 1){
						dup2(pipefds[1], 1);
						close(pipefds[1]);
					}

					execvp(left_argv[0], left_argv);
	  				break;

        		/* Listening child. */
        		case 0 :

	  				/* - Redirect input of "child" through pipe.
					  * - Don't need write side of pipe. Read side dup'ed to stdin.
				  	 * - Exec command on right side of pipe and recursively deal with other pipes
					 *
					 * Fill in code. */

					close(pipefds[1]);
					if(pipefds[0] != 0){
						dup2(pipefds[0], 0);
						close(pipefds[0]);
					}
          			pipe_and_exec(&myArgv[pipe_argv_index+1]);
			}
	}
	perror("Couldn't fork or exec child process");
  	exit(errno);
}
