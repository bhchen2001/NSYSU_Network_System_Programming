/*
 * create hole file for hw1_cp
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define FILE_SIZE 1024
#define HOLE_SIZE 10

void errExit(const char *message){
	perror("message");
	exit(1);
}

int main(void){
	char content[FILE_SIZE] = {0};
	int i = 0, fd;
	off_t lseek_offset = 0;
	ssize_t file_size = 0;

	printf("Enter content for hole file: \n");
	fgets(content, FILE_SIZE, stdin);
	printf("Creating hole file...\n");

	fd = open("hw1_hole_file", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd == -1) errExit("opening hole file");
	
	for(i = 0; i < strlen(content); i++){
		if((file_size = write(fd, &content[i], sizeof(char))) == -1)
			errExit("err when writing");
		lseek_offset = lseek(fd, HOLE_SIZE, SEEK_CUR);
	}

	if(close(fd) == -1)
		errExit("close");
	printf("\"hw1_hole_file\" is created\n");

	return 0;
}
