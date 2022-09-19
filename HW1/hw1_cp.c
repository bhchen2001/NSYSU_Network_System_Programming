/*
 * cp command implementation
 * copy file contains holes
 */

 #include <unistd.h>
 #include <sys/stat.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <fcntl.h>

#define READ_SIZE 1024

void errExit(const char *message){
	perror(message);
	exit(1);
}

int main(int argc, char *argv[]){
	int input_fd, output_fd;
	mode_t mode;
	ssize_t read_size;
	char buffer[READ_SIZE] = {0}, null_ch[READ_SIZE];

	if(argc > 3)
		errExit("too many arguments");
	else if(argc < 3)
		errExit("need 2 arguments");

	if((input_fd = open(argv[1], O_RDONLY)) == -1)
		errExit("input open");
	if((output_fd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC,
								  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
								  S_IROTH | S_IWOTH)) == -1)
		errExit("output open");
	
	while((read_size = read(input_fd, buffer, READ_SIZE)) > 0){
		// read NULL character
		if(buffer == null_ch)
			lseek(output_fd, read_size, SEEK_CUR);
		else{
			if(write(output_fd, buffer, read_size) != read_size) errExit("write output file");
		}
	}

	if(close(input_fd) == -1) errExit("close input file");
	if(close(output_fd) == -1) errExit("close output file");
	
	return 0;
}
