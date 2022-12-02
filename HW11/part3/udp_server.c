/*
 * udp_server : listen on a UDP socket ;reply immediately
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

#include "dict.h"

int main(int argc, char **argv) {
	static struct sockaddr_in server,client;
	int sockfd,siz;
	Dictrec dr, *tryit = &dr;
	int rc, opt = 1;
	int n;

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(errno);
	}

	/* Create a UDP socket.
	 * Fill in code. */
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd == -1) DIE("socket() fail");

	/* Initialize address.
	 * Fill in code. */
	memset(&server, 0, sizeof(server));
	memset(&client, 0, sizeof(client));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	// server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_addr.s_addr = INADDR_ANY;
	rc = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (void *)&opt, (socklen_t)sizeof(opt));
	if(rc == -1) DIE("setsockopt() fail");

	/* Name and activate the socket.
	 * Fill in code. */
	rc = bind(sockfd, (const struct sockaddr *)&server, sizeof(server));
	if(rc == -1) DIE("bind() fail");


	for (;;) { /* await client packet; respond immediately */

		siz = sizeof(client); /* siz must be non-zero */

		/* Wait for a request.
		 * Fill in code. */
		memset(&(dr.word), 0, sizeof(dr.word));
		// fprintf(stderr, "server: waiting for request...\n");
		int size = sizeof(dr);
		n = recvfrom(sockfd, (void *)&dr, sizeof(Dictrec), MSG_WAITALL, (struct sockaddr *)&client, (socklen_t *)&siz);
		if(n == -1) DIE("recvform() fail");

		while (n > 0) {
			/* Lookup request and respond to user. */
			switch(lookup(tryit,argv[1]) ) {
				case FOUND: 
					/* Send response.
					 * Fill in code. */
					n = sendto(sockfd, (void *)&dr, sizeof(Dictrec), 0, (struct sockaddr *)&client, siz);
					if(n == -1) DIE("sendto() fail");
					break;
				case NOTFOUND : 
					/* Send response.
					 * Fill in code. */
					strcpy(tryit->text, "XXXX");
					n = sendto(sockfd, (void *)&dr, sizeof(Dictrec), MSG_CONFIRM, (struct sockaddr *)&client, siz);
					if(n == -1) DIE("sendto() fail");
					break;
				case UNAVAIL:
					DIE(argv[1]);
			} /* end lookup switch */
			memset(&dr, 0, sizeof(Dictrec));
			n = recvfrom(sockfd, (void *)&dr, sizeof(Dictrec), MSG_CONFIRM, (struct sockaddr *)&client, (socklen_t *)&siz);
		} /* end while */
	} /* end forever loop */
	exit(EXIT_SUCCESS);
} /* end main */
