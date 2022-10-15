/*
 * redirect_in.c  :  check for <
 */

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "shell.h"
#define STD_OUTPUT 1
#define STD_INPUT  0

/*
 * Look for "<" in myArgv, then redirect input to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_in(char ** myArgv) {
  	int i = 0;
  	int fd;
	errno = 1;

  	/* search forward for <
  	 *
	 * Fill in code. */

	while(myArgv[i] != (char *)NULL){
		if(strcmp(myArgv[i], "<") == 0){
			errno = 0;
			break;
		}
		i++;
	}
	if(errno == 1) return -1;

  	if (myArgv[i]) {	/* found "<" in vector. */

    	/* 1) Open file.
     	 * 2) Redirect stdin to use file for input.
   		 * 3) Cleanup / close unneeded file descriptors.
   		 * 4) Remove the "<" and the filename from myArgv.
		 *
   		 * Fill in code. */

		fd = open(myArgv[i + 1], O_RDONLY);
		dup2(fd, STD_INPUT);
		close(fd);
		myArgv[i] = (char *)NULL;
  	}
  	return 0;
}
