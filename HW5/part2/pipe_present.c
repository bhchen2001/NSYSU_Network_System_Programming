/*
 *  pipe_present.c :  check for |
 */

#include <stdio.h>
#include "shell.h"

/*
 * Return index offset into argv of where "|" is,
 * -1 if in an illegal position (first or last index in the array),
 * or 0 if not present.
 */
int pipe_present(char ** myCurrentArgv) {
	int index = 0;

  	/* Search through myCurrentArgv for a match on "|". */

  	if /* At the beginning or at the end. */ {
    	return -1;

  	} else if /* Off the end. */ {
    	return 0;

  	} else {
    	/* In the middle. */
    	return index;
  	}
}
