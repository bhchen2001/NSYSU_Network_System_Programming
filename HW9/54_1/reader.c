/*
 * implement writer-reader application by POSIX shared memory
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

typedef enum { FALSE, TRUE } Boolean;

#define SHM_KEY 0x1234
#define SEM_KEY 0x5678

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

#define WRITE_SEM 0
#define READ_SEM 1

#define SHM_NAME "shm_hw"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

struct shmseg{
	int cnt;
	char buf[BUF_SIZE];
};

Boolean bsUseSemUndo = FALSE;
Boolean bsRetryOnEintr = TRUE;

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	#if defined(_linux_)
		struct seminfo *__buf;
	#endif
};

int initSemAvailable(int semId, int semNum)
{
    union semun arg;

    arg.val = 1;
    return semctl(semId, semNum, SETVAL, arg);
}

int initSemInUse(int semId, int semNum)
{
    union semun arg;

    arg.val = 0;
    return semctl(semId, semNum, SETVAL, arg);
}

int reserveSem(int semId, int semNum)
{
    struct sembuf sops;

    sops.sem_num = semNum;
    sops.sem_op = -1;
    sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

    while (semop(semId, &sops, 1) == -1)
        if (errno != EINTR || !bsRetryOnEintr)
            return -1;

    return 0;
}

int releaseSem(int semId, int semNum)
{
    struct sembuf sops;

    sops.sem_num = semNum;
    sops.sem_op = 1;
    sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

    return semop(semId, &sops, 1);
}

int main(int argc, char *argv[]){
	int semid, shmid, bytes, xfrs;
	struct shmseg *shmp;

	if((semid = semget(SEM_KEY, 0, 0)) == -1){
		perror("semget");
		exit(1);
	}

	if((shmid = shm_open(SHM_NAME, O_RDONLY, 0)) == -1){
		perror("shm_open");
		exit(1);
	}

	if((shmp = mmap(NULL, sizeof(struct shmseg), PROT_READ, MAP_SHARED, shmid, 0)) == (void *)-1){
		perror("mmap");
		exit(1);
	}
	

	for(xfrs = 0, bytes = 0; ; xfrs++){
		if(reserveSem(semid, READ_SEM) == -1){
			perror("reserveSem");
			exit(1);
		}
		if(shmp->cnt == 0){
			break;
		}
		bytes += shmp->cnt;
		
		if(write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt){
			perror("write");
			exit(1);
		}
		if(releaseSem(semid, WRITE_SEM) == -1){
			perror("releaseSem");
			exit(1);
		}
	}

	if(munmap(shmp, sizeof(struct shmseg)) == -1){
		perror("munmap");
		exit(1);
	}

	if(releaseSem(semid, WRITE_SEM) == -1){
		perror("releaseSem");
		exit(1);
	}

	fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
	exit(EXIT_SUCCESS);
}
