/* 
 * client program of 52_3
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
	pid_t client_pid = getpid();
	int req_file_fd, read_size, receive_byte_num = 0, receive_msg_num = 0;
	struct request_msg client_req;
	struct response_msg client_res;
	struct mq_attr attr;
	
	if(argc != 2){
		perror("invalid argument number");
		exit(1);
	}

	client_req.pid = client_pid;
	strcpy(client_req.pathname, argv[1]);

	snprintf(mq_client_name, 1024, "%s%d", CLIENT_MQ_NAME, client_pid);
	fprintf(stderr, "client_name: %s\n", mq_client_name);


	memset(&attr, 0, sizeof(struct mq_attr));
	attr.mq_msgsize = sizeof(struct response_msg);
	attr.mq_maxmsg = 4;

	if((mq_client = mq_open(mq_client_name, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR, &attr)) == (mqd_t)-1){
		perror("mq_open");
		exit(1);
	}

	if((mq_server = mq_open(SERVER_MQ_NAME, O_WRONLY)) == (mqd_t)-1){
		perror("mq_open");
		exit(1);
	}

	if(mq_send(mq_server, (char *)&client_req, sizeof(struct request_msg), 0) == -1){
		perror("mq_send");
		exit(1);
	}

	while(1){
		if((read_size = mq_receive(mq_client, (char *)&client_res, sizeof(struct response_msg), 0)) == -1){
			perror("mq_receive");
			exit(1);
		}
		if(client_res.data[0] == NULL){
			break;
		}
		receive_byte_num += strlen(client_res.data);
		// receive_byte_num += read_size;
		receive_msg_num += 1;
		// fprintf(stderr, "message get\n");
	}
	fprintf(stderr, "Reiceived %d bytes (%d messages)\n", receive_byte_num, receive_msg_num);

	if(mq_close(mq_server) == -1){
		perror("mq_close");
		exit(1);
	}
	if(mq_close(mq_client) == -1){
		perror("mq_client");
		exit(1);
	}
	if(mq_unlink(mq_client_name) == -1){
		perror("mq_client_name");
		exit(1);
	}

	exit(EXIT_SUCCESS);

}
