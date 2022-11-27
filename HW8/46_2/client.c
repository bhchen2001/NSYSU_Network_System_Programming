/*
 * client application for message queue implementation
 * sent a seq-len to server
 * usage: ./client <seq-len>
 */

#include "msgqueue_seqnum.h"
#include <unistd.h>
#include <limits.h>

int main(int argc, char *argv[]){
	struct request client_req;
	struct response client_res;
	int seq_len, msgId;
	char *endptr;
	pid_t pid;

	if(argc == 1) seq_len = 1;
	else{
		seq_len = strtol(argv[1], &endptr, 10);
		if(seq_len < 0){
			perror("invalid seq_len");
			exit(1);
		}
	}

	if((msgId = msgget(QUEUE_KEY, S_IRUSR | S_IWUSR)) == -1){
		perror("msgget");
		exit(1);
	}

	pid = getpid();
	
	/* set request information */
	client_req.mtype = MSG_TYPE;
	client_req.clientId = pid;
	client_req.seqLen = seq_len;

	if(msgsnd(msgId, &client_req, REQ_SIZE, 0) == -1){
		perror("msgsnd");
		exit(1);
	}
	/* receive the msg with pid as msgtyp */
	if(msgrcv(msgId, &client_res, RES_SIZE, pid, 0) == -1){
		perror("msgrcv");
		exit(1);
	}

	fprintf(stderr, "received seq number: %d\n", client_res.seqNum);
	exit(EXIT_SUCCESS);
	
}
