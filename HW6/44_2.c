/* 
 * implement popen and pclose
 * argument: const char *command, const char *type
 * return type: FILE *
 * usage: ./44_2 <command> <type>
 */

#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define FD_SIZE 1024
#define COMMAND_SIZE 1024
#define BUFFER_SIZE 1024
#define STDIN_FILENO 0
#define STDOUT_FILENO 1

static int fd_array[FD_SIZE];
void init_fd_array(){
	int i;
	for(i= 0; i < FD_SIZE; i++)
		fd_array[i] = -1;
}

static FILE *my_popen(const char *command, const char *type){
	int pfd[2];
	pid_t pid;
	FILE *fd_caller;

	if(pipe(pfd) == -1){
		perror("pipe in my_popen");
		return NULL;
	}

	switch(pid = fork()){
		case -1:
			perror("fork in my_popen");
			return NULL;
		default:
			/*
			 * parent process
			 * close unused pipe and return the another end to caller
			 * store the child pid into fd_array with fd as index (for my_pclose())
			 * return the FILE stream pointer associate to the pipe end
			 */
			if(strcmp(type, "r") == 0){
				if(close(pfd[1]) == -1){
					perror("parent - close pfd in my_popen");
					return NULL;
				}

				if(pfd[0] >= FD_SIZE){
					perror("parent - exceed the FD_SIZE");
					return NULL;
				}
				fd_array[pfd[0]] = pid;
				fd_caller = fdopen(pfd[0], type);
				// write(STDOUT_FILENO, "add\n", 4);
			}
			else if(strcmp(type, "w") == 0){
				if(close(pfd[0]) == -1){
					perror("parent - close pfd in my_popen");
					return NULL;
				}
				if(pfd[1] >= FD_SIZE){
					perror("parent - exceed the FD_SIZE");
					return NULL;
				}
				fd_array[pfd[1]] = pid;
				fd_caller = fdopen(pfd[1], type);
			}
			return fd_caller;
		case 0:
			/* 
			 * child process
			 * joint the pipe to the STD FILE pointer, depend on the type 'r'/'w'
			 * pass the command to "/bin/sh" using -c flag (see popen(3) man page)
			 */
			// execl("/bin/sh", "/bin/sh", "-c", command, (char*)NULL);
			if(strcmp(type, "r") == 0){
				if(close(pfd[0]) == -1){
					perror("child - close pfd in my_popen");
					return NULL;
				}
				// defensive check for dup2
				if(pfd[1] != STDOUT_FILENO){
					if(dup2(pfd[1], STDOUT_FILENO) == -1){
						perror("child - dup2 in my_popen");
						return NULL;
					}
					if(close(pfd[1]) == -1){
						perror("child - close pfd in my_popen");
						return NULL;
					}
				}
			}
			else if(strcmp(type, "w") == 0){
				if(close(pfd[1]) == -1){
					perror("child - close pfd in my_popen");
					return NULL;
				}
				if(pfd[0] != STDIN_FILENO){
					if(dup2(pfd[0], STDIN_FILENO) == -1){
						perror("child - dup2 in my_popen");
						return NULL;
					}
					if(close(pfd[0]) == -1){
						perror("child - close pfd in my_popen");
						return NULL;
					}
				}
			}
			execl("/bin/sh", "sh", "-c", command, (char *)NULL);
			return NULL;
	}
}

int my_pclose(FILE *stream){
	int fd;
	if((fd = fileno(stream)) == -1){
		perror("fileno in my_pclose");
		return -1;
	}
	if(fd_array[fd] == -1){
		/* no child process associate with this file descriptor */
		perror("no child process found");
		return -1;
	}
	if(close(fd) == -1){
		perror("close in my_pclose");
		exit(1);
	}
	int stat;
	if(waitpid(fd_array[fd], &stat, 0) == -1){
		perror("waitpid");
		return -1;
	}
	return WEXITSTATUS(stat);
}

int main(int argc, char *argv[]){
	if(argc < 3 || argc > 3){
		perror("wrong element of argument");
		exit(1);
	}
	char type[COMMAND_SIZE], command[COMMAND_SIZE];
	strcpy(type, argv[2]);
	strcpy(command, argv[1]);
	if(strcmp(type, "w") != 0 && strcmp(type, "r") != 0){
		perror("invalid mode");
		exit(1);
	}
	// initilize the fd_array which keeps the child process id
	init_fd_array();
	// write(STDOUT_FILENO, "-\n", 2);
	FILE *command_f;
	char buf[BUFFER_SIZE];
	ssize_t read_size;
	if((command_f = my_popen(command, type)) == NULL){
		perror("my_popen");
		exit(1);
	}

	if(strcmp(type, "r") == 0){
		/* read from command_f and print to STDOUT */
		int fd;
		if((fd = fileno(command_f)) == -1){
			perror("fileno");
			exit(1);
		}
		if((read_size = read(fd, buf, BUFFER_SIZE)) == -1){
			perror("read");
			exit(1);
		}
		if(write(STDOUT_FILENO, buf, read_size) == -1){
			perror("write");
			exit(1);
		}
	}
	else if(strcmp(type, "w") == 0){
		/* write to command_f */
		int fd;
		if((fd = fileno(command_f)) == -1){
			perror("fileno");
			exit(1);
		}
		/* terminated by ctrl+D */
		while(fgets(buf, BUFFER_SIZE, stdin) != NULL){
			// if(buf[strlen(buf) - 1] == '\n')
			// 	buf[strlen(buf) - 1] = '\0';
			if(write(fd, buf, strlen(buf)) == -1){
				perror("write");
				exit(1);
			}
		}
		// write(STDOUT_FILENO, "done\n", 5);
		/*
		if((read_size = read(STDIN_FILENO, buf, BUFFER_SIZE)) == -1){
			perror("read");
			exit(1);
		}
		if((write(fd, buf, BUFFER_SIZE)) == -1){
			perror("write");
			exit(1);
		}
		*/
	}

	// write(STDOUT_FILENO, "---\n", 4);
	if(my_pclose(command_f) == -1){
		perror("my_pclose");
		exit(1);
	}

	return 0;
}
