/* 
 * header for message queue - seq_num application
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <fcntl.h>

#define QUEUE_KEY 0x1aaaaaa1

struct request{
	long mtype;
	int clientId;
	int seqLen;
};

#define REQ_SIZE (offsetof(struct request, seqLen) - offsetof(struct request, clientId) + sizeof(int))

struct response{
	long mtype;
	int seqNum;
};

#define RES_SIZE (sizeof(int))
#define MSG_TYPE 1
