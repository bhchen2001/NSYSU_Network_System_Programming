/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.h"




/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/

/* "echo" command.  Does not print final <CR> if "-n" encountered. */
static void bi_echo(char **argv) {
  	/* Fill in code. */
	char buffer[100];
	if(strcmp(argv[1], "-n") == 0){
		int string_num, index = 3;
		string_num = atoi(argv[2]) + 2;
		while(index < string_num && argv[index] != (char *)NULL)
			index += 1;
		if(argv[index] == (char *)NULL){
			perror("-n value too big");
			exit(0);
		}
		snprintf(buffer, 100, "%s\n", argv[index]);
		if(write(STDOUT_FILENO, buffer, strlen(buffer)) == -1){
			perror("write");
			exit(0);
		}
	}
	else{
		int index = 1;
		while(argv[index] != (char *)NULL){
			snprintf(buffer, 100, "%s ", argv[index]);
			if(write(STDOUT_FILENO, buffer, strlen(buffer)) == -1){
				perror("write");
				exit(0);
			}
			index += 1;
		}
		snprintf(buffer, 100, "\n");
		if(write(STDOUT_FILENO, buffer, strlen(buffer)) == -1){
			perror("write");
			exit(0);
		}
	}

}
/* Fill in code. */




/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
	char * keyword;				/* When this field is argv[0] ... */
	void (* do_it)(char **);	/* ... this function is executed. */
} inbuilts[] = {

	/* Fill in code. */
	{ "exit", exit},
	{ "quit", exit},
	{ "logout", exit},
	{ "bye", exit},
	{ "echo", bi_echo },		/* When "echo" is typed, bi_echo() executes.  */
	{ NULL, NULL }				/* NULL terminated. */
};




/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd * this; 		/* close coupling between is_builtin & do_builtin */

/* Check to see if command is in the inbuilts table above.
Hold handle to it if it is. */
int is_builtin(char *cmd) {
  	struct cmd *tableCommand;

  	for (tableCommand = inbuilts ; tableCommand->keyword != NULL; tableCommand++)
    	if (strcmp(tableCommand->keyword,cmd) == 0) {
			this = tableCommand;
			return 1;
		}
  	return 0;
}


/* Execute the function corresponding to the builtin cmd found by is_builtin. */
int do_builtin(char **argv) {
  	this->do_it(argv);
}
