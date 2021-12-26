/* Program to demonstrate server side of UDP communication */
/* There is one argument, the UDP port number to bind to */
/* This variation illustrate gethostbyaddr, to identify
	the John's */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <stdlib.h>

main (argc,argv) 
	int argc;
	char *argv[];
{
	
	struct sockaddr_in sin; /* structure for socket address */
	int s;
	int fd;
	int len, sum,i;
	struct hostent *hostentp;

		/* set up IP addr and port number for bind */
	sin.sin_addr.s_addr= INADDR_ANY;
	sin.sin_port = atoi(argv[1]);
	sin.sin_family= AF_INET;

		/* Get an internet socket for Datgram connections */
	if ((s = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
		perror("Socket");
		exit(1);
		}

		/* Do the actual bind */
	if (bind (s, (struct sockaddr *) &sin, sizeof (sin)) <0) {
		perror("bind");
		exit(2);
		}


	sum=0;
int j =0;

	while(1) {

		len = sizeof(sin);
		recvfrom(s, &i, sizeof(i), 0, (struct sockaddr *) &sin, &len);
		if (i != -1) {sum += i; j++;}
		else {
			hostentp=gethostbyaddr((char *)&sin.sin_addr.s_addr,
						sizeof(sin.sin_addr.s_addr),AF_INET);
			printf("This John (%s) sent %d nums, adding up to %d \n",hostentp->h_name,j, sum);
			sum =0;
			j=0;
			}
	}
}
