/*
 * lookup9 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet UDP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_in server;
	struct hostent *host;
	static int first_time = 1;
	int rc;
	char *addr_str;

	if (first_time) {  /* Set up server address & create local UDP socket */
		first_time = 0;

		/* Set up destination address.
		 * Fill in code. */
		memset(&server, 0, sizeof(server));
		server.sin_family = AF_INET;
		server.sin_port = htons(PORT);
		// host = gethostbyname(resource);
		// if(host == NULL) DIE("gethostbyname() fail");
		// addr_str = inet_ntoa(*(struct in_addr *)host->h_addr);
		// strcat(addr_str, "\0");
		// fprintf(stderr, "addr_str: %s\n", addr_str);
		// server.sin_addr.s_addr = inet_addr(addr_str);
		// server.sin_addr.s_addr = inet_addr("127.0.0.1");
		server.sin_addr.s_addr = INADDR_ANY;

		/* Allocate a socket.
		 * Fill in code. */
		sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(sockfd == -1) DIE("socket() fail");
		// connect(sockfd, (struct sockaddr  *)&server, sizeof(server));
	}

	/* Send a datagram & await reply
	 * Fill in code. */
	socklen_t len = sizeof(server);
	rc = sendto(sockfd, (void *)sought, sizeof(Dictrec), MSG_CONFIRM, (struct sockaddr *)&server, len);
	if(rc == -1) DIE("sendto() fail");
	rc = recvfrom(sockfd, (void *)sought, sizeof(Dictrec), 0, (struct sockaddr *)&server, (socklen_t *)&len);
	if(rc == -1) DIE("recvfrom() fail");

	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
