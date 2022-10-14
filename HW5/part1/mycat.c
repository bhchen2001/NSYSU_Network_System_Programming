/* 
 * simple version of cat command
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>


#define BUFFER_SIZE 128

void help(int status){
	fprintf(stderr, "Usage: mycat <filename>");
	exit(status);
}

int main(int argc, char *argv[]){
	if(argc != 2) help(1);

	char *buf = NULL;
	FILE *fp;
	size_t len = 0;
	ssize_t read_size;

	if((fp = fopen(argv[1], "r")) == NULL){
		perror("open");
		exit(1);
	}

	while((read_size = getline(&buf, &len, fp)) != -1){
		if(write(STDOUT_FILENO, buf, strlen(buf)) == -1){
			perror("write");
			exit(1);
		}
	}

	return 0;
}
