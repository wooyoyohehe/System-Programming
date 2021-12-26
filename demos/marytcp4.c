/* Program to demonstrate server side of TCP communication */
/* There is one argument, the TCP port number to bind to */
/* This deemo illustrates using the POLL system call to monitor
   several fd's at one time in a single process */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
//#include <stropts.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (argc,argv) 
	int argc;
	char *argv[];
{
	
	struct sockaddr_in sin; /* structure for socket address */
	int s;
	int fd;
	int len, i,num,count;
	struct pollfd pollarray[6];     /* up to 1+5 simultaneous connections*/
	int sum[6];						/* sum [1..5] for the 5 johns */
	int johns[6][40];				/*names of johns*/
	struct hostent *hostentp;

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

		/* Initialize the pollarray */
	pollarray[0].fd=s;     /* Accept Socket*/
	pollarray[0].events=POLLIN;
						  /* 5 possible john's */
	for (i=1;i<=5;i++) {pollarray[i].fd=-1;pollarray[i].events=POLLIN;}

	while(1) {
		poll(pollarray,6,-1);   /* no timeout, blocks until some activity*/

				/* Check first for new connection */
		if (pollarray[0].revents & POLLIN) {
			
			len=sizeof(sin);
			if ((fd= accept (s, (struct sockaddr *) &sin, &len)) <0) {
				perror ("accept");
					exit(3);
				}
					/* Find first available slot for new john */
			for (i=1;i<=5;i++) if (pollarray[i].fd == -1) break;
				pollarray[i].fd=fd;
				sum[i]=0;
				hostentp=gethostbyaddr((char *)&sin.sin_addr.s_addr,
					sizeof(sin.sin_addr.s_addr),AF_INET);
				strcpy((char *)&johns[i][0], hostentp->h_name);
			}

				/* If there are no new connections, process waiting john's */
		else for (i=1;i<=5;i++) {
			if ((pollarray[i].fd !=-1) && pollarray[i].revents) {
				count=read(pollarray[i].fd,&num,4);
				if (count==4) sum[i] +=ntohl(num);
				else {
					printf("John %d (%s) has sum %d\n",i,(char *)&johns[i][0], sum[i]);
					sum[i]=0;
					close(pollarray[i].fd);
					pollarray[i].fd = -1;
					}
				}
			}
		}
}

