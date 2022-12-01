/*
 * usock_server : listen on a Unix socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include "dict.h"

int main(int argc, char **argv) {
    struct sockaddr_un server;
    int sd,cd,n;
    Dictrec tryit;
	int rc;

    if (argc != 3) {
      fprintf(stderr,"Usage : %s <dictionary source>"
          "<Socket name>\n",argv[0]);
      exit(errno);
    }

    /* Setup socket.
     * Fill in code. */
	sd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sd == -1) DIE("socket() fail");
    
    /* Initialize address.
     * Fill in code. */
	memset(&server, 0, sizeof(struct sockaddr_un));
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, argv[2]);
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
					// fprintf(stderr, "server receive word: %s\n", tryit.word);
					/* Lookup request. */
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to client. */
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

				/* Terminate child process.  It is done. */
				exit(0);

			/* Parent continues here. */
			default :
				close(cd);
				break;
		} /* end fork switch */
    } /* end forever loop */
} /* end main */
