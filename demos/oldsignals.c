/* Program to illustrate the use of traditional  signals on UNIX 
		The program runs a computational loop to 
		compute perfect numbers starting at a fixed
		point.

	A time alarm signal is used to periodically print status
	An interrupt signal is used for status on demand
	A quit signal is used to reset the test interval (or terminate) */

#include <signal.h>
#include <errno.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void perfect (int);

jmp_buf jmpenv; /* environment saved by setjmp*/

int n; /* global variable indicating current test point */

int main() {
	
	int begin; /* starting point for next search*/
		/* interrupt routines*/
	void status();
	void query();

	if (setjmp(jmpenv)) {
		printf("Enter search starting point (0 to terminate): ");
		scanf("%d",&begin);
		if (begin==0) exit(0);
		}
	else begin=2;
	
	/* Status Routine will handle timer and INTR */

	signal(SIGINT, status);
	signal(SIGALRM, status);
	signal(SIGQUIT, query);

	/* start alarm clock */
	alarm(20);
		perfect(begin);
		}

void perfect(start) 
	int start;
{
	int i,sum;

	n=start;

while (1) {
	sum=1;
	for (i=2;i<n;i++)
		if (!(n%i)) sum+=i;
    
	if (sum==n) printf("%d is perfect\n",n);
	n++;
	}
}

void
status(signum)
	int signum;

{
	if ( signal(signum, SIG_IGN) == SIG_ERR) {
		perror("Status SIG_IGN");
		exit(1);
		}

	alarm(0); /* shutoff alarm */

	if (signum == SIGINT) printf("Interrupt ");
	if (signum == SIGALRM) printf("Timer ");

	printf("processing %d\n",n);

	alarm(20);	/*restart alarm*/
	if (signal(signum, status) == SIG_ERR) {
		perror("Status reset");
		exit(2);
		}
}	

void query() {longjmp(jmpenv,1);}
