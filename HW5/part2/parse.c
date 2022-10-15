/*
 * parse.c : use whitespace to tokenise a line
 * Initialise a vector big enough
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

/* Parse a commandline string into an argv array. */
char ** parse(char *line) {

  	static char delim[] = " \t\n"; /* SPACE or TAB or NL */
  	int count = 0;
  	char * token;
  	char **newArgv;

  	/* Nothing entered. */
  	if (line == NULL || strcmp(line,"\n")==0) {
    	return NULL;
  	}

  	/* Init strtok with commandline, then get first token.
     * Return NULL if no tokens in line.
	 *
	 * Fill in code.
     */

	token = strtok(line, delim);
	if(token == (char *)NULL) return NULL;


  	/* Create array with room for first token.
  	 *
	 * Fill in code.
	 */

	if((newArgv = (char **)malloc(sizeof(char *))) == NULL){
		perror("malloc char **");
		exit(1);
	}
	if((newArgv[count] = (char *)malloc(100*sizeof(char))) == NULL){
		perror("malloc char *");
		exit(1);
	}
	strcpy(newArgv[count++], token);


  	/* While there are more tokens...
	 *
	 *  - Get next token.
	 *	- Resize array.
	 *  - Give token its own memory, then install it.
	 * 
  	 * Fill in code.
	 */
	while(token){
		token = strtok(NULL, delim);
		if(token == (char *)NULL) break;
		if((newArgv = realloc(newArgv, (count + 1)*sizeof(char *))) == NULL){
			perror("realloc char **");
			exit(1);
		}
		if((newArgv[count] = malloc(100 * sizeof(char))) == NULL){
			perror("realloc char *");
			exit(1);
		}
		strcpy(newArgv[count++], token);
	}


  	/* Null terminate the array and return it.
	 *
  	 * Fill in code.
	 */

	if((newArgv = realloc(newArgv, (count + 1)*sizeof(char *))) == NULL){
		perror("realloc char **");
		exit(1);
	}
	if((newArgv[count] = malloc(1 * sizeof(char))) == NULL){
		perror("realloc char *");
		exit(1);
	}
	newArgv[count] = (char *)NULL;


  	return newArgv;
}


/*
 * Free memory associated with argv array passed in.
 * Argv array is assumed created with parse() above.
 */
void free_argv(char **oldArgv) {

	int i = 0;

	/* Free each string hanging off the array.
	 * Free the oldArgv array itself.
	 *
	 * Fill in code.
	 */

	while(oldArgv[i] != (char *)NULL){
		free(oldArgv[i]);
		i++;
	}

	free(oldArgv);
}
