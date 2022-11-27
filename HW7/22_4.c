/*
 * System V functions implementation
 * 1. sigset()
 * 2. sighold()
 * 3. sigrelse()
 * 4. sigignore()
 * 5. sigpause()
 */

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

typedef void (*sighandler_t) (int);

void handler(int sig){
	printf("signal arrived at handler\n");
}

void kill_proc(int sig){
	printf("kill the process");
	exit(1);
}

int mySighold(int sig){
	int result;
	sigset_t block;
	sigemptyset(&block);
	sigaddset(&block, sig);

	result = sigprocmask(SIG_BLOCK, &block, NULL);
	return result;
}

int mySigrelse(int sig){
	int result;
	sigset_t unblock;
	sigemptyset(&unblock);
	sigaddset(&unblock, sig);

	result = sigprocmask(SIG_UNBLOCK, &unblock, NULL);
	return result;
}

sighandler_t mySigset(int sig, sighandler_t handler){
	struct sigaction sa, old_sa;
	sigset_t block_sig;
	if(handler == SIG_DFL){
		/* reset the disposition of sig to default */
		sa.sa_flags = SA_RESETHAND;
		sa.sa_handler = handler;
		if(sigaction(sig, &sa, &old_sa) == -1){
			perror("sigaction");
			return (sighandler_t) -1;
		}
	}
	else if(handler == SIG_IGN){
		/* ignore sig */
		sa.sa_handler = SIG_IGN;
		if(sigaction(sig, &sa, &old_sa) == -1){
			perror("sigaction");
			return (sighandler_t) -1;
		}
	}
	// else if(handler == SIG_HOLD){
	// 	/* add sig to signal mask, but leave the disposition unchanged */
	// 	sa.sa_handler = handler;
	// 	if(sigaction(sig, &sa, &old_sa) == -1){
	// 		perror("sigaction");
	// 		return (sighandler_t) -1;
	// 	}
	// 	mySighold(sig);
	// 	return old_sa.sa_handler;
	// }
	else{
		/* other signal handler function */
		sa.sa_handler = handler;
		if(sigaction(sig, &sa, &old_sa) == -1){
			perror("sigaction");
			return (sighandler_t) -1;
		}
	}
	/* 
	 * check if the sig was blocked before the call
	 * if sig was blocked before, return SIG_HOLD
	 * else return previous disposition
	 */
	if(sigprocmask(SIG_BLOCK, NULL, &block_sig) == -1){
		perror("sigprocmask");
			return (sighandler_t) -1;
	}
	/* if the handler is anything other than SIG_HOLD, remove the sig from the signal mask */
	if(sigismember(&block_sig, sig)){
		mySigrelse(sig);
		// return SIG_HOLD;
		return old_sa.sa_handler;
	}
	else{
		return old_sa.sa_handler;
	}
}

int mySigignore(int sig){
	int result;
	struct sigaction ignore_action;
	ignore_action.sa_handler = SIG_IGN;
	
	result = sigaction(sig, &ignore_action, NULL);
	return result;
}

int mySigpause(int sig){
	sigset_t curr_mask;
	if(sigprocmask(SIG_SETMASK, NULL, &curr_mask) == -1){
		perror("sigprocmask");
		exit(1);
	}

	sigdelset(&curr_mask, sig);
	return sigsuspend(&curr_mask);
}

int main(int argc, char *argv[]){
	char func[10], output[1024];
	if(argc != 2){
		perror("invalid argument number");
		exit(1);
	}
	strcpy(func, argv[1]);

	if(strcmp(func, "sigset") == 0){
		if(mySigset(SIGINT, handler) == -1){
			perror("mySigset");
			exit(1);
		}
		printf("please press ctrl-C\n");
		pause();
	}
	else if(strcmp(func, "sighold") == 0){
		if(mySighold(SIGINT) == -1){
			perror("mySighold");
			exit(1);
		}
		if(mySigset(SIGTSTP, kill_proc) == -1){
			perror("mySigset");
			exit(1);
		}
		fprintf(stderr, "the SIGINT signal is blocked, press ctrl-C\n");
		fprintf(stderr, "to kill the process, press ctrl-Z\n");
		pause();
	}
	else if(strcmp(func, "sigrelse") == 0){
		fprintf(stderr, "first block SIGINT for 3 secs\n");
		if(mySighold(SIGINT) == -1){
			perror("mySighold");
			exit(1);
		}
		sleep(3);
		fprintf(stderr, "next relse it\n");
		if(mySigrelse(SIGINT) == -1){
			perror("mySigrelse");
			exit(1);
		}
		fprintf(stderr, "no pending signal, now SIGINT is unblocked\n");
		pause();
	}
	else if(strcmp(func, "sigignore") == 0){
		if(mySigignore(SIGINT) == -1){
			perror("mySigignore");
			exit(1);
		}
		if(mySigset(SIGTSTP, kill_proc) == -1){
			perror("mySigset");
			exit(1);
		}
		fprintf(stderr, "the SIGINT signal is ignore, press ctrl-C\n");
		fprintf(stderr, "to kill the process, press ctrl-Z\n");
		pause();
	}
	else if(strcmp(func, "sigpause") == 0){
		struct sigaction sa;
		sa.sa_handler = handler;
		if(sigaction(SIGINT, &sa, NULL) == -1){
			perror("sigaction");
			exit(1);
		}
		fprintf(stderr, "pause SIGINT signal...\n");
		mySigpause(SIGINT);
	}
	else{
		perror("invalid function");
		exit(1);
	}
	exit(EXIT_SUCCESS);
}
