/* Program to demonstrate server side of TCP communication */
/* There is one argument, the TCP port number to bind to */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdlib.h>

main (argc,argv) 
	int argc;
	char *argv[];
{
	
	struct sockaddr_in sin; /* structure for socket address */
	int s;
	int fd;
	int len, sum,i;

		/* set up IP addr and port number for bind */
	sin.sin_addr.s_addr= INADDR_ANY;
	sin.sin_port = atoi(argv[1]);
	sin.sin_family= AF_INET;

		/* Get an internet socket for stream connections */
	if ((s = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("Socket");
		exit(1);
		}

		/* Do the actual bind */
	if (bind (s, (struct sockaddr *) &sin, sizeof (sin)) <0) {
		perror("bind");
		exit(2);
		}

		/* Allow a connection queue for up to 5 JOHNS */
	listen(s,5);

		/* Now loop accepting connections */
	while (1) {
		if ((fd= accept (s, (struct sockaddr *) &sin, &len)) <0) {
			perror ("accept");
			exit(3);
			}

		if (fork() == 0) { /* CHILD now does the work */
			sum=0;
			while ( read(fd,&i,sizeof (int)) == sizeof (int)) sum += i;
			printf("This John adds up to %d \n",sum);
			exit(0);
			}
	}
}
