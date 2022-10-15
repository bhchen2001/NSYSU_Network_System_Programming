/*
 * is_background.c :  check for & at end
 */

#include <stdio.h>
#include "shell.h"

int is_background(char ** myArgv) {

  	if (*myArgv == NULL)
    	return 0;

  	/* Look for "&" in myArgv, and process it.
  	 *
	 *	- Return TRUE if found.
	 *	- Return FALSE if not found.
	 *
	 * Fill in code.
	 */

	int index = 0;
	while(myArgv[index] != (char *)NULL){
		if(strcmp(myArgv[index], "&") == 0){
			return TRUE;
		}
		index++;
	}

	return FALSE;

}