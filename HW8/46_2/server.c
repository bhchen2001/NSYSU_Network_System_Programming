/*
 * server application for message queue implementation
 * receive request from client and return a seq number
 * Usage: ./server
 */

#include "msgqueue_seqnum.h"
#include <signal.h>

int msgId;

void handler(int sig){
	if(msgctl(msgId, IPC_RMID, NULL) == -1){
		perror("msgctl");
		exit(1);
	}
	fprintf(stderr, "remove message queue\n");
	exit(0);
}

int main(int argc, char *argv[]){
	struct request server_req;
	struct response server_res;
	int seq_num = 0;


	if((msgId = msgget(QUEUE_KEY, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR)) == -1){
		perror("msgget");
		exit(1);
	}

	fprintf(stderr, "message queue is set...\n");

	struct sigaction sa;
	sa.sa_handler = handler;
	if(sigaction(SIGINT, &sa, NULL) == -1){
		perror("sigaction");
		exit(1);
	}

	fprintf(stderr, "for exit and kill msg queue, press Ctrl+C\n");

	while(1){
		if(msgrcv(msgId, &server_req, REQ_SIZE, MSG_TYPE, 0) == -1){
			perror("msgget");
			exit(1);
		}

		fprintf(stderr, "request get, clientId = %d, seqLen = %d\n", server_req.clientId, server_req.seqLen);
		server_res.mtype = server_req.clientId;
		server_res.seqNum = seq_num;

		if(msgsnd(msgId, &server_res, RES_SIZE, 0) == -1){
			perror("msgsnd");
			exit(1);
		}
		fprintf(stderr, "response sent: %d\n", server_req.clientId);
		seq_num += server_req.seqLen;
	}
}
