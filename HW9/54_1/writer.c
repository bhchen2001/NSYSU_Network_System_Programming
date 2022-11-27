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

int main(int argc, char *argv[])
{
    int shmid, semid, bytes, xfrs;
    struct shmseg *shmp;
    union semun dummy;

    semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);
    if (semid == -1){
    	perror("semget");
		exit(1);
	}

    if (initSemAvailable(semid, WRITE_SEM) == -1){
        perror("initSemAvailable");
		exit(1);
	}
    if (initSemInUse(semid, READ_SEM) == -1){
        perror("initSemInUse");
		exit(1);
	}

	shmid = shm_open(SHM_NAME, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    if (shmid == -1){
		perror("shm_open");
		exit(1);
	}

    if (ftruncate(shmid, sizeof(struct shmseg)) == -1){
        perror("ftruncate");
		exit(1);
	}

    shmp = mmap(NULL, sizeof(struct shmseg), PROT_READ|PROT_WRITE, MAP_SHARED, shmid, 0);
    if (shmp == (void *) -1){
        perror("mmap");
		exit(1);
	}

    for (xfrs = 0, bytes = 0; ; xfrs++, bytes += shmp->cnt) {
        if (reserveSem(semid, WRITE_SEM) == -1){
            perror("reserveSem");
			exit(1);
		}

        shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);
        if (shmp->cnt == -1){
            perror("read");
			exit(1);
		}

        if (releaseSem(semid, READ_SEM) == -1){
            perror("releaseSem");
			exit(1);
		}

        if (shmp->cnt == 0)
            break;
    }

    if (reserveSem(semid, WRITE_SEM) == -1){
        perror("reserveSem");
		exit(1);
	}

    if (semctl(semid, 0, IPC_RMID, dummy) == -1){
        perror("semctl");
		exit(1);
	}
    if (munmap(shmp, sizeof(struct shmseg) ) == -1){
        perror("munmap");
		exit(1);
	}

    if(shm_unlink(SHM_NAME)==-1){
        perror("shm_unlink");
		exit(1);
	}

    fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
