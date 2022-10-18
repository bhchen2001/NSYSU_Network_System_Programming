/*
 * read the output from ls over pipe
 */

#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 2048

int main(int argc, char *argv[]){
	int pd[2];
	char buf[BUFFER_SIZE] = {NULL};
	char *command[3] = {"ls", "-la", (char*)NULL};

	if(pipe(pd) == -1){
		perror("pipe");
		exit(1);
	}

	switch(fork()){
		case -1:
			perror("fork");
			exit(1);
			break;
		case 0:
			// child
			close(pd[0]);
			close(STDOUT_FILENO);
			dup(pd[1]);
			execvp(command[0], command);
			close(pd[1]);
			exit(0);
			break;
		default:
			// parent
			break;
	}

	close(pd[1]);
	read(pd[0], buf, BUFFER_SIZE);
	if(write(STDOUT_FILENO, buf, strlen(buf)) == -1){
		perror("write");
		exit(1);
	}
	close(pd[0]);

	return 0;
}
