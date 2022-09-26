/*
 * tail command implementation
 * 1. prints last few lines from file
 * 
 * options:
 * 		-n option: print last lines
 */
 
 #include <sys/stat.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include <errno.h>

#define BUF_SIZE (1024 * 4)


void errExit(const char *message){
	fprintf(stderr, message);
	exit(EXIT_FAILURE);
}

 int main(int argc, char *argv[]){
 	int option = 0, fd, line_num = 10, line_count = 0, offset = 0;
	off_t file_size;
	char *endptr = NULL, *file, buffer[BUF_SIZE];
	while((option = getopt(argc, argv, "n:")) != -1){
		switch(option){
			case 'n':
				/* 
				 * if the value pass to atoi() cannot be represented, 
				 * the behavior is undefined, use strtol instead
				 */
				
				// reset errno before calling strtol
				errno = 0;
				line_num = strtol(optarg, &endptr, 10);
				if(errno != 0) errExit("error in strtol\n");
				else if(*endptr != 0) errExit("error in strtol: detect additional character\n");
				else if (line_num < 0) errExit("line num must grater than 0\n");
				// else printf("line_num = %d\n", line_num);
				break;
			case '?': 
				errExit("Usage: tail [-n num] file\n");
				break;
		}
	}

	file = argv[optind];
	if(file == NULL) errExit("missing file\n");
	if((fd = open(file, O_RDONLY)) == -1) errExit("error when opening file");
	if((file_size = lseek(fd, 0, SEEK_END)) == -1) errExit("error when lseek\n");

	int i = 0, seek_size;
	ssize_t read_size;
	off_t cur;
	line_num += 1;
	
	(BUF_SIZE < file_size) ? (seek_size = BUF_SIZE) : (seek_size = file_size);
	if((cur = lseek(fd, -1 * (seek_size), SEEK_END)) == -1) errExit("error when calling lseek\n");

	while(line_count < line_num){
		if((read_size = read(fd, buffer, BUF_SIZE)) == -1) errExit("error when reading file");

		for(i = read_size - 1; i > 0; i--){
			if(buffer[i] == '\n') line_count++;
			if(line_count == line_num){
				offset += (read_size - i);
				// printf("offset = %d\n", offset);
				break;
			}
		}

		if(line_count == line_num){
			offset -= 1;
			// printf("find all the line\n");
			break;
		}
		else if(read_size < BUF_SIZE){
			// printf("nothing can read from the file, not enough line\n");
			offset = file_size;
			break;
		}
		else{
			offset += seek_size;
			if((cur = lseek(fd, -1 * (seek_size), SEEK_CUR)) == -1)
				errExit("error when lseek (back to cur before read)\n");
			(BUF_SIZE < cur) ? (seek_size = BUF_SIZE) : (seek_size = cur);
			if((cur = lseek(fd, -1 * (seek_size), SEEK_CUR)) == -1)
				errExit("error when lseek (read next part from end)\n");
		}
	}

	// printf("line_count = %d\n", line_count);
	// printf("offset = %d\n", offset);

	if((cur = lseek(fd, -1 * (offset), SEEK_END)) == -1)
		errExit("error when lseek (print to stdout)\n");

	while((read_size = read(fd, buffer, BUF_SIZE)) > 0){
		if(write(STDOUT_FILENO, buffer, read_size) != read_size)
			errExit("write error\n");
	}

	if(close(fd) == -1) errExit("error when closing fd\n");
	
	return 0;
 }
