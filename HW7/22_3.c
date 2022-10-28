#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

void handler(int sig){
	printf("get signal\n");
}

int main(int argc, char *argv[]){
	int sig_num = 0, sig;
	siginfo_t si;
	sigset_t block_mask;
	// clock_t start = clock();
	struct timeval start, end;
	gettimeofday(&start, NULL);
	if(argc != 2){
		perror("invalid argument number");
		exit(1);
	}
	sig_num = strtol(argv[1], (char **)NULL, 10);

	if (sig_num <= 0){
		perror("invalid <sig_num>");
		exit(1);
	}
	printf("sig_num: %d\n", sig_num);
	
	/* block the signal SIGUSR avoid default deposition */
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	if(sigaction(SIGUSR1, &sa, NULL) == -1){
		perror("sigaction");
		exit(1);
	}

	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGUSR1);
	if(sigprocmask(SIG_SETMASK, &block_mask, NULL) == -1){
		perror("sigprocmask");
		exit(1);
	}

	pid_t child_pid = fork();
	switch(child_pid){
		case -1:
			perror("fork");
			exit(1);
		case 0:
			/* child process: send sig to parent and suspend */
			for(int scnt = 0; scnt < sig_num; scnt++){
				if(kill(getppid(), SIGUSR1) == -1){
					perror("kill");
					exit(1);
				}
				if((sig = sigwaitinfo(&block_mask, &si)) == -1){
					perror("sigwaitinfo");
					exit(1);
				}
			}
			exit(EXIT_SUCCESS);
		default:
			/* parent process: suspend and send sig to child */
			for(int scnt = 0; scnt < sig_num; scnt++){
				if((sig = sigwaitinfo(&block_mask, &si)) == -1){
					perror("sigwaitinfo");
					exit(1);
				}
				if(kill(child_pid, SIGUSR1) == -1){
					perror("kill");
					exit(1);
				}
			}
			gettimeofday(&end, NULL);
			printf("time interval: %d", end.tv_sec - start.tv_sec);
	}

	// clock_t interval = clock() - start;
	// printf("clock_per_sec: %d\n", CLOCKS_PER_SEC);
	// printf("start: %d, %d\n", start, clock());
	// printf("time interval = %d\n", interval / CLOCKS_PER_SEC);
	exit(EXIT_SUCCESS);
}
