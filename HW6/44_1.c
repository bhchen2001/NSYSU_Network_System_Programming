#include <sys/wait.h>
#include <stdio.h>

#define BUFFER_SIZE 128
#define STDIN 0
#define STDOUT 1


int main(int argc, char *argv[]){
	int pfd_1[2], pfd_2[2], i;
	char buf[BUFFER_SIZE] = {NULL}, output[BUFFER_SIZE];
	ssize_t read_size;

	snprintf(output, BUFFER_SIZE, "Enter the input:");

	/* write from parent, read from child */
	if(pipe(pfd_1) == -1){
		perror("pipe");
		exit(1);
	}

	/* read form parent, write from child */
	if(pipe(pfd_2) == -1){
		perror("pipe");
		exit(1);
	}

	switch(fork()){
		case -1:
			perror("fork");
			exit(1);
		case 0:
			/* child: read from pfd_1, write to pfd_2 */
			if(close(pfd_1[1]) == -1){
				perror("child - close pfd_1");
				exit(1);
			}
			if(close(pfd_2[0]) == -1){
				perror("child - close pfd_2");
				exit(1);
			}
			while(1){
				if((read_size = read(pfd_1[0], buf, BUFFER_SIZE)) == -1){
					perror("child - read pfd_1");
					exit(1);
				}
				if(read_size == 0)
					// when parent close write in pfd_1
					break;
				for(i = 0; i < strlen(buf); i++){
					if(buf[i] < 'a' || buf[i] > 'z')
						continue;
					buf[i] = buf[i] - 'a' + 'A';
				}
				if(write(pfd_2[1], buf, strlen(buf)) == -1){
					perror("child - write pfd_2");
					exit(1);
				}
			}
			if(close(pfd_2[1]) == -1){
				perror("child - close pfd_2");
				exit(1);
			}
			if(close(pfd_1[0]) == -1){
				perror("child - close pfd_1");
				exit(1);
			}
			exit(0);
		default:
			/* parent: write to pfd_1, read from pfd_2 */
			if(close(pfd_1[0]) == -1){
				perror("parent - close pfd_1");
				exit(1);
			}
			if(close(pfd_2[1]) == -1){
				perror("parent - close pfd_2");
				exit(1);
			}
			if(write(STDOUT, output, strlen(output)) == -1){
				perror("parent - write STDOUT");
				exit(1);
			}
			if((read_size = read(STDIN, buf, BUFFER_SIZE)) == -1){
				perror("parent - read STDIN");
				exit(1);
			}
			if(write(pfd_1[1], buf, strlen(buf)) == -1){
				perror("parent - write pfd_1");
				exit(1);
			}
			if(close(pfd_1[1]) == -1){
				perror("parent - close pfd_1");
				exit(1);
			}
			if((read_size = read(pfd_2[0], buf, BUFFER_SIZE)) == -1){
				perror("parent - read pfd_2");
				exit(1);
			}
			if(write(STDOUT, buf, strlen(buf)) == -1){
				perror("parent - write STDOUT");
				exit(1);
			}
			/*
			if(close(pfd_1[1]) == -1){
				perror("parent - close pfd_1");
				exit(1);
			}
			*/
			if(close(pfd_2[0]) == -1){
				perror("parent - close pfd_2");
				exit(1);
			}
			wait(NULL);
	}
	return 0;
}
