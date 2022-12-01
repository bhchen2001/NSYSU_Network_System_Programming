/*
 * lookup7 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Unix TCP Sockets
 * The name of the socket is passed as resource.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_un server;
	static int first_time = 1;
	int n;
	int rc;

	if (first_time) {     /* connect to socket ; resource is socket name */
		first_time = 0;

		/* Set up destination address.
		 * Fill in code. */
		sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
		if(sockfd == -1) DIE("socket() fail");

		/* Allocate socket. */
		memset(&server, 0, sizeof(struct sockaddr_un));
		server.sun_family = AF_UNIX;
		strcpy(server.sun_path,resource);

		/* Connect to the server.
		 * Fill in code. */
		rc = connect(sockfd, (struct sockaddr *)&server, sizeof(server));
		if(rc == -1) DIE("connect() fail");
	}

	/* write query on socket ; await reply
	 * Fill in code. */
	rc = send(sockfd, (void *)sought, sizeof(Dictrec), 0);
	if(rc == -1) DIE("send() fail");

	rc = recv(sockfd, (void *)sought, sizeof(Dictrec), 0);
	if(rc == -1) DIE("recv() fail");

	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;;
}
