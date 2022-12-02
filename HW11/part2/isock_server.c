/*
 * isock_server : listen on an internet socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#include "dict.h"

int main(int argc, char **argv) {
	static struct sockaddr_in server;
	int sd,cd,n;
	Dictrec tryit;
	int rc, opt = 1;

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(1);
	}

	/* Create the socket.
	 * Fill in code. */
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd == -1) DIE("socket() fail");

	/* Initialize address.
	 * Fill in code. */
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	rc = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	if(rc == -1) DIE("setsockopt() fail");
	rc = bind(sd, (struct sockaddr *)&server, sizeof(server));
	if(rc == -1) DIE("bind() fail");

	/* Name and activate the socket.
	 * Fill in code. */
	rc = listen(sd, 10);
	if(rc == -1) DIE("listen() fail");

	/* main loop : accept connection; fork a child to have dialogue */
	for (;;) {

		/* Wait for a connection.
		 * Fill in code. */
		int len = sizeof(server);
		cd = accept(sd, (struct sockaddr *)&server, &len);
		if(cd == -1) DIE("accept() fail");

		/* Handle new client in a subprocess. */
		switch (fork()) {
			case -1 : 
				DIE("fork");
			case 0 :
				close (sd);	/* Rendezvous socket is for parent only. */
				/* Get next request.
				 * Fill in code. */
				while (recv(cd, (void *)&tryit, sizeof(tryit), 0) > 0) {
					/* Lookup the word , handling the different cases appropriately */
					// fprintf(stderr, "server receive word: %s\n", tryit.word);
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to the client. */
						case FOUND:
							/* Fill in code. */
							rc = send(cd, (void *)&tryit, sizeof(tryit), 0);
							if(rc == -1) DIE("send() fail");
							break;
						case NOTFOUND:
							/* Fill in code. */
							strcpy(tryit.text, "XXXX");
							rc = send(cd, (void *)&tryit, sizeof(tryit), 0);
							if(rc == -1) DIE("send() fail");
							break;
						case UNAVAIL:
							DIE(argv[1]);
					} /* end lookup switch */
				} /* end of client dialog */
				exit(0); /* child does not want to be a parent */

			default :
				close(cd);
				break;
		} /* end fork switch */
	} /* end forever loop */
} /* end main */
