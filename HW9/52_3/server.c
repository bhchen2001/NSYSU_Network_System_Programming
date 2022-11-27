/* 
 * server program of 52_3
 */

#include <mqueue.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define PATH_MAX 1024
#define MSG_SIZE 2048
#define SERVER_MQ_NAME "/server_mq"
#define CLIENT_MQ_NAME "/client_mq"


struct request_msg{
	pid_t pid;
	char pathname[PATH_MAX];
};

struct response_msg{
	char data[MSG_SIZE];
};
int main(int argc, char *argv[]){
	mqd_t mq_server, mq_client;
	char mq_client_name[1024] = {NULL};
	int req_file_fd, read_size;
	struct request_msg server_req;
	struct response_msg server_res;
	struct mq_attr attr;

	memset(&attr, 0, sizeof(struct mq_attr));	
	attr.mq_msgsize = sizeof(struct request_msg);
	attr.mq_maxmsg = 4;

	if((mq_server = mq_open(SERVER_MQ_NAME, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR, &attr)) == (mqd_t)-1){
		perror("mq_open");
		exit(1);
	}

	fprintf(stderr, "mq_server opened\n");

	while(1){
		if(mq_receive(mq_server, (char *)&server_req, sizeof(struct request_msg), 0) == -1){
			perror("mq_receive");
			exit(1);
		}

		/* 
		 * get the actual client mq name
		 */
		snprintf(mq_client_name, 1024, "%s%d", CLIENT_MQ_NAME, server_req.pid);
		fprintf(stderr, "mq_client_name: %s\n", mq_client_name);
		if((mq_client = mq_open(mq_client_name, O_WRONLY)) == (mqd_t)-1){
			perror("mq_open");
			continue;
		}
		
		fprintf(stderr, "received file name: %s\n", server_req.pathname);

		if((req_file_fd = open(server_req.pathname, O_RDONLY)) == -1){
			perror("open request file");
			continue;
		}

		/* 
		 * send the content of file to client by mq_send
		 */
		while((read_size = read(req_file_fd, server_res.data, MSG_SIZE - 1)) > 0){
			if(mq_send(mq_client, (char *)&server_res, sizeof(struct response_msg), 0) == -1){
				perror("mq_send");
				break;
			}
			memset(&server_res, 0, sizeof(struct response_msg));
		}

		if(close(req_file_fd) == -1){
			perror("close");
			exit(1);
		}

		/*
		 * signify the client with NULL message
		 */
		// memset(server_res.data, '\0', MSG_SIZE);
		server_res.data[0] = '\0';
		if(mq_send(mq_client, (char *)&server_res, sizeof(struct response_msg), 0) == -1){
			perror("mq_send");
			continue;
		}
		if(mq_close(mq_client) == -1){
			perror("mq_close");
			continue;
		}
	}
	
	if(mq_close(mq_server) == -1){
		perror("mq_close");
		exit(1);
	}

	if(mq_unlink(SERVER_MQ_NAME) == -1){
		perror("mq_unlink");
		exit(1);
	}

	exit(EXIT_SUCCESS);

}
