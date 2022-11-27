/* 
 * implement cp(1) by using mmap() and memcpy()
 */

#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int src_fd, des_fd;
	void *src_map, *des_map;
	struct stat file_stat;
	if(argc != 3){
		perror("invalid argument number");
		exit(1);
	}
	if((src_fd = open(argv[1], O_RDONLY)) == -1){
		perror("open src");
		exit(1);
	}

	/*
	 * obatin the size of input file
	 */
	if(fstat(src_fd, &file_stat) == -1){
		perror("fstat");
		exit(1);
	}

	/*
	 * allocate the memory mappnig to file
	 */
	if((src_map = mmap((caddr_t)NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, src_fd, (off_t)0)) == MAP_FAILED){
		perror("mmap src");
		exit(1);
	}
	if(close(src_fd) == -1){
		perror("close src");
		exit(1);
	}
	
	if((des_fd = open(argv[2], O_CREAT | O_RDWR, file_stat.st_mode)) == -1){
		perror("open des");
		exit(1);
	}

	/* 
	 * set the size of the output file
	 */
	if(ftruncate(des_fd, file_stat.st_size) == -1){
		perror("ftruncate");
		exit(1);
	}
	if((des_map = mmap((caddr_t)NULL, file_stat.st_size, PROT_WRITE, MAP_SHARED, des_fd, (off_t)0)) == MAP_FAILED){
		perror("mmap des");
		exit(1);
	}
	if(close(des_fd) == -1){
		perror("close des");
		exit(1);
	}

	/*
	 * copy src's mem to des's mem
	 */
	memcpy(des_map, src_map, file_stat.st_size);

	if(munmap(src_map, file_stat.st_size) == -1){
		perror("munmap");
		exit(1);
	}
	if(munmap(des_map, file_stat.st_size) == -1){
		perror("munmap");
		exit(1);
	}

	exit(EXIT_SUCCESS);
}
