/* Program to demonstrate server side of TCP communication */
/* There is one argument, the TCP port number to bind to */
/* This variation illustrate gethostbyaddr, to identify
	the John's and catches SIGCHLD and handles byte order */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <stdlib.h>
#include <signal.h>

main (argc,argv) 
	int argc;
	char *argv[];
{
	
	struct sockaddr_in sin; /* structure for socket address */
	int s;
	int fd;
	int len, sum,i;
	struct hostent *hostentp;

	void waitchild(int);
	signal(SIGCHLD, waitchild);

		/* set up IP addr and port number for bind */
	sin.sin_addr.s_addr= INADDR_ANY;
	sin.sin_port = htons(atoi(argv[1]));
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
		len=sizeof(sin);
		if ((fd= accept (s, (struct sockaddr *) &sin, &len)) <0) {
			perror ("accept");
			exit(3);
			}

		if (fork() == 0) { /* CHILD now does the work */
			sum=0;
			while ( read(fd,&i,sizeof (int)) == sizeof (int)) sum +=htonl(i);
			hostentp=gethostbyaddr((char *)&sin.sin_addr.s_addr,
						sizeof(sin.sin_addr.s_addr),AF_INET);
			printf("This John (%s) adds up to %d \n",hostentp->h_name,sum);
			exit(0);
			}
	}
}

void waitchild(int sig) { int status; wait(&status);}
