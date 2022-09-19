/* 
 * tee command implementation
 * 1. read stdin until EOF
 * 2. write copy to stdout
 * 3. write copy to file
 *
 * option: -a option (append text to file)
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define READ_BUF 1024

void errExit(const char *message){
	perror("message");
	exit(1);
}

int main(int argc, char *argv[]){
	int i=0, option, fd, flags;
	ssize_t read_size;
	char buffer[READ_BUF] = {0};
	int append = 0;
	mode_t mode;
	while((option = getopt(argc, argv, "+a")) != -1){
		switch(option){
			case 'a': append = 1; break;
		}
	}

	if((read_size = read(STDIN_FILENO, buffer, READ_BUF)) == -1)
		errExit("read");


	mode = S_IRUSR | S_IWUSR;

	if(append == 1)
		flags = O_CREAT | O_WRONLY | O_APPEND;
	else
		flags = O_CREAT | O_WRONLY | O_TRUNC;

	if(write(STDOUT_FILENO, buffer, read_size) != read_size)
		errExit("write");
	for(i = optind; i < argc; i++){
		fd = open(argv[i], flags, mode);
		if(write(fd, buffer, read_size) != read_size)
			errExit("write");
		if(close(fd) == -1)
			errExit("close");
	}

	return 0;
}
