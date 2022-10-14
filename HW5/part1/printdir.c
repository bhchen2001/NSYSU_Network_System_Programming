/*
 * print the current directory
 * determine the buffer size to pass to the getcwd()
 */

#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[]){
	char *buffer = NULL;

	if((buffer = getcwd(NULL, 0)) == NULL){
		perror("getcwd");
		exit(1);
	}

	if((write(STDOUT_FILENO, buffer, strlen(buffer))) == -1){
		perror("write");
		exit(1);
	}

	return 0;
}
