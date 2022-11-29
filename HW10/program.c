/* 
 * producer-consumer application using shared memory
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
#include <signal.h>
#include <time.h>
#include <math.h>

typedef enum {FALSE, TRUE} Boolean;

#define SHM_KEY 0x1234
#define SEM_KEY 0x5678

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

#define WRITE_SEM 0
#define READ_SEM 1

#define SHM_NAME "shm_hw"
#define SHM_NAME_CNT "shm_hw_cnt"

#define BUFSIZE 80

struct shmseg{
	int msg_num;
	char buf[BUFSIZE];
};

struct shmseg_cnt{
	int cnt;
	int loss_cnt;
};

Boolean beUseSemUndo = FALSE;
Boolean beRetryOnEintr = TRUE;

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	# if defined(_linux_)
		struct seminfo *__buf;
	#endif
};

int initSemAvailable(int semId, int semNum){
	union semun arg;

	arg.val = 1;
	return semctl(semId, semNum, SETVAL, arg);
}

int initSemInUse(int semId, int semNum){
	union semun arg;

	arg.val = 0;
	return semctl(semId, semNum, SETVAL, arg);
}

int reserveSem(int semId, int semNum){
	struct sembuf sops;

	sops.sem_num = semNum;
	sops.sem_op = -1;
	sops.sem_flg = beUseSemUndo ? SEM_UNDO : 0;

	while(semop(semId, &sops, 1) == -1)
		if(errno != EINTR || !beRetryOnEintr)
			return -1;
	return 0;
}

int releaseSem(int semId, int semNum){
	struct sembuf sops;

	sops.sem_num = semNum;
	sops.sem_op = 1;
	sops.sem_flg = beUseSemUndo ? SEM_UNDO : 0;

	return semop(semId, &sops, 1);
}


void handler(int signo, siginfo_t *info, void *ctx){
	printf("signal get\n");
}

int total_msg_get = 0;

int main(int argc, char *argv[]){

	if(argc != 5){
		perror("invalid argument number");
		exit(1);
	}
	char *endptr;
	long m = strtol(argv[1], &endptr, 10);
	long r = strtol(argv[2], &endptr, 10);
	long n = strtol(argv[3], &endptr, 10);
	long b = strtol(argv[4], &endptr, 10);

	fprintf(stderr, "information: M: %d R: %d N: %d B: %d\n", m, r, n, b);

	int shmid, shmid_cnt, consumer_index;
	struct shmseg *shmp;
	struct shmseg_cnt *shmp_cnt;
	
	/* 
	 * create the shared memory and set the size
	 */
	if((shmid = shm_open(SHM_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1){
		perror("shm_open");
		exit(1);
	}
	if(ftruncate(shmid, b * sizeof(struct shmseg)) == -1){
		perror("ftruncate");
		exit(1);
	}
	if((shmid_cnt = shm_open(SHM_NAME_CNT, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1){
		perror("shm_open");
		exit(1);
	}
	if(ftruncate(shmid_cnt, sizeof(struct shmseg_cnt)) == -1){
		perror("ftruncate");
		exit(1);
	}

	if((shmp = mmap(NULL, b * sizeof(struct shmseg), PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0)) == (void *)-1){
		perror("mmap");
		exit(1);
	}
	if((shmp_cnt = mmap(NULL, sizeof(struct shmseg_cnt), PROT_READ | PROT_WRITE, MAP_SHARED, shmid_cnt, 0)) == (void *)-1){
		perror("mmap");
		exit(1);
	}
	memset(&(shmp_cnt->cnt), 0, sizeof(int));
	memset(&(shmp_cnt->loss_cnt), 0, sizeof(int));

	/* block the signal SIGUSR avoid default deposition */
	int sig;
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO;
	if(sigaction(SIGUSR1, &sa, NULL) == -1){
		perror("sigaction");
		exit(1);
	}

	sigset_t block_mask;
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGUSR1);
	if(sigprocmask(SIG_SETMASK, &block_mask, NULL) == -1){
		perror("sigprocmask");
		exit(1);
	}

	pid_t pid_array[n];
	pid_t pid;

	for(consumer_index = 0; consumer_index < n; consumer_index++){
		switch(pid = fork()){
			case -1:
				perror("fork");
				exit(1);
			case 0:
				/*
				 * consumer wait for the signal and read memory
				 */
				if((shmp = mmap(NULL, b * sizeof(struct shmseg), PROT_READ, MAP_SHARED, shmid, 0)) == (void *)-1){
					perror("mmap_consumer");
					exit(1);
				}
				if((shmp_cnt = mmap(NULL, sizeof(struct shmseg_cnt), PROT_WRITE, MAP_SHARED, shmid_cnt, 0)) == (void *)-1){
					perror("mmap_consumer_shmid_cnt");
					exit(1);
				}
				
				/* 
				 * wait for the signal
				 */
				siginfo_t si;
				while(1){
					if((sig = sigwaitinfo(&block_mask, &si)) == -1){
						perror("sigwaitinfo");
						exit(1);
					}
					// fprintf(stderr, "consumer %d receive signal %d\n", consumer_index, si.si_value.sival_int);
					int message_index = si.si_value.sival_int%b;
					int msg_num = shmp[message_index].msg_num;
					if(si.si_value.sival_int == msg_num){
						/* correct message get */
						// fprintf(stderr, "consumer %d get the message %d\n", consumer_index, si.si_value.sival_int);
						shmp_cnt->cnt += 1;
						usleep(400);
					}
					else{
						/* message loss */
						// fprintf(stderr, "consumer %d loss message %d (shmp[%d]_msg_num: %d, %s)\n",consumer_index, si.si_value.sival_int, message_index, shmp[message_index].msg_num, shmp[message_index].buf);
						shmp_cnt->loss_cnt += 1;
					}
					if(si.si_value.sival_int == m - 1) break;
				}

				if(munmap(shmp, b * sizeof(struct shmseg)) == -1){
					perror("munmap");
					exit(1);
				}
				if(munmap(shmp_cnt, sizeof(struct shmseg_cnt)) == -1){
					perror("munmap");
					exit(1);
				}
				exit(EXIT_SUCCESS);
			default:
				pid_array[consumer_index] = pid;
				// fprintf(stderr, "pid: %d\n", pid);
				break;
		}
	}
	/*
	 * producer sent the message
	 */
	int i;
	for(i = 0; i < m; i++){
		struct timespec start, end;
		long ms = 0;
		clock_gettime(CLOCK_REALTIME, &start);
		snprintf(shmp[(i%b)].buf, BUFSIZE, "This is message %d", i);
		shmp[(i%b)].msg_num = i;
		// fprintf(stderr, "write to shmp[%d]: %d\n", (i%b), shmp[(i%b)].msg_num);

		union sigval mysigval;
		mysigval.sival_int = i;
		int sig_index;
		for(sig_index = 0; sig_index < n; sig_index ++){
			// fprintf(stderr, "pid_array[%d]: %d\n", sig_index, pid_array[sig_index]);
			// fprintf(stderr, "mysigval_int: %d\n", mysigval.sival_int);
			if(sigqueue(pid_array[sig_index], SIGUSR1, mysigval) == -1){
				perror("sigqueue");
				exit(1);
			}
			clock_gettime(CLOCK_REALTIME, &end);
			ms = ((end.tv_nsec - start.tv_nsec) / 1.0e6);
			/* if time sending signal is larger than R, stop sending signal*/
			if(ms > r){
				fprintf(stderr, "timeout, sent until consumer %d", i);
				break;
			}
		}
		/* if there still time remain, sleep until then*/
		if(ms < r) usleep(r - ms);
	}


	if(wait() == -1){
		perror("wait");
		exit(1);
	}

	double total_msg = (m * n);
	double loss_rate = 1-((shmp_cnt->cnt) / total_msg);
	fprintf(stderr, "Total messages: %d\nSum of received messages by all consumers: %d\nLoss rate: %lf\n", (m * n), shmp_cnt->cnt, loss_rate);
	// fprintf(stderr, "loss message num: %d\n", shmp_cnt->loss_cnt);

	if(munmap(shmp, b * sizeof(struct shmseg)) == -1){
		perror("munmap");
		exit(1);
	}
	if(munmap(shmp_cnt, sizeof(struct shmseg_cnt)) == -1){
		perror("munmap");
		exit(1);
	}

	if(shm_unlink(SHM_NAME) == -1){
		perror("shm_unlink");
		exit(1);
	}
	exit(EXIT_SUCCESS);
}
