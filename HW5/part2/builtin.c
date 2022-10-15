/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h>
#include "shell.h"

/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/
static void bi_builtin(char ** argv);	/* "builtin" command tells whether a command is builtin or not. */
static void bi_cd(char **argv) ;		/* "cd" command. */
static void bi_echo(char **argv);		/* "echo" command.  Does not print final <CR> if "-n" encountered. */
static void bi_hostname(char ** argv);	/* "hostname" command. */
static void bi_id(char ** argv);		/* "id" command shows user and group of this process. */
static void bi_pwd(char ** argv);		/* "pwd" command. */
static void bi_quit(char **argv);		/* quit/exit/logout/bye command. */




/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
  	char * keyword;					/* When this field is argv[0] ... */
  	void (* do_it)(char **);		/* ... this function is executed. */
} inbuilts[] = {
  	{ "builtin",    bi_builtin },   /* List of (argv[0], function) pairs. */

    /* Fill in code. */
    { "echo",       bi_echo },
    { "quit",       bi_quit },
    { "exit",       bi_quit },
    { "bye",        bi_quit },
    { "logout",     bi_quit },
    { "cd",         bi_cd },
    { "pwd",        bi_pwd },
    { "id",         bi_id },
    { "hostname",   bi_hostname },
    {  NULL,        NULL }          /* NULL terminated. */
};


static void bi_builtin(char ** argv) {
	/* Fill in code. */
	int i = 0;
	char output[BUFFER_SIZE];
	for(i = 0; i < 10; i++){
		if(strcmp(argv[1], inbuilts[i].keyword) == 0){
			snprintf(output, BUFFER_SIZE, "%s is a builtin feature\n", argv[1]);
			if((write(STDOUT_FILENO, output, strlen(output))) == -1){
				perror("bi_builtin write");
				exit(1);
			}
			break;
		}
	}
	if(i >= 10){
		snprintf(output, BUFFER_SIZE, "%s is NOT a builtin feature\n", argv[1]);
		if((write(STDOUT_FILENO, output, strlen(output))) == -1){
			perror("bi_builtin write");
			exit(1);
		}
	}
}

static void bi_cd(char **argv) {
	/* Fill in code. */
	if(chdir(argv[1]) == -1){
		perror("chdir");
		exit(1);
	}
}

static void bi_echo(char **argv) {
	/* Fill in code. */
	int index = 1;
	char output[BUFFER_SIZE];
	while(argv[index] != NULL){
		if(write(STDOUT_FILENO, argv[index], strlen(argv[index])) == -1){
			perror("echo write");
			exit(1);
		}
	}
}

static void bi_hostname(char ** argv) {
	/* Fill in code. */
	struct utsname buf;
	if((uname(&buf)) == -1){
		perror("uname");
		exit(1);
	}

	char output[strlen(buf.sysname) + 1];
	strcpy(output, buf.sysname);
	strcat(output, "\n");
	if((write(STDOUT_FILENO, output, strlen(output))) == -1){
		perror("write");
		exit(1);
	}
}

static void bi_id(char ** argv) {
 	/* Fill in code. */
	uid_t uid;
	gid_t gid;
	long hostid;
	struct passwd *pwuid;
	struct group *g;
	char output[BUFFER_SIZE];
	uid = getuid();
	gid = getgid();
	if((pwuid = getpwuid(uid)) == NULL){
		perror("getpwuid");
		exit(1);
	}
	if((g = getgrgid(gid)) == NULL){
		perror("getgrgid");
		exit(1);
	}
	snprintf(output, BUFFER_SIZE, "UserID = %d(%s), GroupID = %d(%s)\n", uid, pwuid->pw_name, gid, g->gr_name);
	if((write(STDOUT_FILENO, output, strlen(output))) == -1){
		perror("write");
		exit(1);
	}
}

static void bi_pwd(char ** argv) {
	/* Fill in code. */
	char *dir = NULL;
	if((dir = getcwd(NULL, 0)) == NULL){
		perror("getcwd");
		exit(1);
	}

	char output[strlen(dir) + 1];
	strcpy(output, dir);
	strcat(output, "\n");
	if((write(STDOUT_FILENO, output, strlen(output))) == -1){
		perror("write");
		exit(1);
	}
}

static void bi_quit(char **argv) {
	exit(0);
}


/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd * this; /* close coupling between is_builtin & do_builtin */

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
