#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_FILE "fifo_file_44_7"

void mymkfifo(void){
    mkfifo(FIFO_FILE, 0666);

	fprintf(stderr, "make fifo_file done\n");
}

int main(int argc, char *argv[]){
    int writeFd, readFd, read_size;
    char write_msg[] = "Hello World";
    char read_msg[1024] = {NULL};
    mymkfifo();
    /* case 1 */
    fprintf(stderr, "==========================\ncase1 : nonblock write open\n(read side close)\n");
    if((writeFd = open(FIFO_FILE, O_WRONLY | O_NONBLOCK)) == -1){
		perror("opening fifo for writing fail");
	}
    fprintf(stderr, "==========================\n");

    fprintf(stderr, "case2 : nonblock read open\n(write side close)\n");
    if(readFd = open(FIFO_FILE, O_RDONLY | O_NONBLOCK) == -1){
		perror("open fifo file fail");
		exit(1);
	}
    fprintf(stderr, "read open success\n");
    fprintf(stderr, "==========================\n");

    fprintf(stderr, "case3 : nonblock write open\n(read side open)\n");
    if((writeFd = open(FIFO_FILE, O_WRONLY | O_NONBLOCK)) == -1){
		perror("opening fifo for writing fail");
		exit(1);
	}
    fprintf(stderr, "write open success\n");
    if(write(writeFd, write_msg, strlen(write_msg) + 1) == -1){
		perror("write");
		exit(1);
	}
	fprintf(stderr, "writing content to fifo done\n");
    fprintf(stderr, "==========================\n");
    if(close(readFd) == -1){
        perror("close");
        exit(1);
    }

    fprintf(stderr, "case4 : nonblock read open\n(write side open)\n");
    if(readFd = open(FIFO_FILE, O_RDONLY | O_NONBLOCK) == -1){
		perror("open fifo file fail");
		exit(1);
	}
    fprintf(stderr, "read open success\n");
    if((read_size = read(readFd, read_msg, 1024)) == -1){
		perror("read");
		exit(1);
	}
    if(read_size == 0) fprintf(stderr, "no content from writing side in fifo\n");
	else fprintf(stderr, "read msg: %s\n", read_msg);
    fprintf(stderr, "==========================\n");

    if(close(readFd) == -1){
        perror("close");
        exit(1);
    }
    if(close(writeFd) == -1){
        perror("close");
        exit(1);
    }
}