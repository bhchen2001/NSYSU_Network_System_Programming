/*
 * print out the day and time
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>


#define BUFFER_SIZE 128

int main(int argc, int *argv[]){
	time_t t = time(NULL);
	struct tm *date_info;
	char output[BUFFER_SIZE];

	date_info = localtime(&t);
	if((date_info = localtime(&t)) == -1){
		perror("localtime");
		exit(1);
	}

	size_t ret = strftime(output, BUFFER_SIZE, "%b %e (%a), %Y %I:%M %p", date_info);
	

	if((write(STDOUT_FILENO, output, strlen(output))) == -1){
		perror("write");
		exit(1);
	}

	return 0;
}
