/*
 * parse.c : use whitespace to tokenise a line
 * Initialise a vector big enough
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.h"

/* Parse a commandline string into an argv array. */
char ** parse(char *line) {

  	static char delim[] = " \t\n"; /* SPACE or TAB or NL */
  	int count = 0;
  	char * token;
  	char **newArgv;
	char argc[80];

  	/* Nothing entered. */
  	if (line == NULL) {
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
		perror("malloc char **\n");
		exit(0);
	}
	if((newArgv[count] = (char *)malloc(100*sizeof(char))) == NULL){
		perror("malloc char *\n");
		exit(0);
	}
	strcpy(newArgv[count++], token);
	char buffer[100];
	snprintf(buffer, 100, "[%d]: %s\n", count-1, token);
	if(write(STDOUT_FILENO, buffer, strlen(buffer)) == -1){
		perror("write");
		exit(0);
	}

  	/* While there are more tokens...
	 *
	 *  - Get next token.
	 *	- Resize array.
	 *  - Give token its own memory, then install it.
	 * 
  	 * Fill in code.
	 */

	while(token){
		token = strtok((char *)NULL, delim);
		if(token == (char *)NULL) break;
		if((newArgv = realloc(newArgv, (count + 1)*sizeof(char *))) == NULL){
			perror("realloc");
			exit(0);
		}
		if((newArgv[count] = malloc(100 * sizeof(char))) == NULL){
			perror("malloc char *");
			exit(0);
		}
		strcpy(newArgv[count++], token);
        snprintf(buffer, 100, "[%d]: %s\n", count-1, token);
		if(write(STDOUT_FILENO, buffer, strlen(buffer)) == -1){
			perror("write");
			exit(0);
		}
	}


  	/* Null terminate the array and return it.
	 *
  	 * Fill in code.
	 */
	if((newArgv = realloc(newArgv, (count + 1)*sizeof(char *))) == NULL){
		perror("realloc");
		exit(0);
	}
	if((newArgv[count] = malloc(100 * sizeof(char))) == NULL){
		perror("malloc char *");
		exit(0);
	}
	newArgv[count] = (char *) NULL;

  	return newArgv;
}


/*
 * Free memory associated with argv array passed in.
 * Argv array is assumed created with parse() above.
 */
void free_argv(char **oldArgv) {

	int index = 0;

	/* Free each string hanging off the array.
	 * Free the oldArgv array itself.
	 *
	 * Fill in code.
	 */

	while(oldArgv[index] != (char *)NULL){
		free(oldArgv[index]);
		index++;
	}

	free(oldArgv);
}
