/* 
 * print out sysyem information
 */

#include <sys/utsname.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 128

int main(int argc, int *argv[]){
	struct utsname *buf;
	long hostid;
	char output[128];
	
	if((uname(buf)) == -1){
		perror("uname");
		exit(1);
	}
	hostid = gethostid();

	snprintf(output, BUFFER_SIZE, "hostname: %s\n%s\nhostid: %ld\n", buf->sysname, buf->release, hostid);

	if(write(STDOUT_FILENO, output, strlen(output)) == -1){
		perror("write");
		exit(1);
	}
}
