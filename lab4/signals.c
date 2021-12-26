/* Program to illustrate the use of POSIX signals on UNIX 
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

void perfect(int);

sigjmp_buf jmpenv; /* environment saved by setjmp*/

int n; /* global variable indicating current test point */

int main() {
	
	int begin; /* starting point for next search*/
		/* interrupt routines*/
	void status();
	void query();

	sigset_t mask;
	struct sigaction action;


	if (sigsetjmp(jmpenv,0)) {
		printf("Enter search starting point (0 to terminate): ");
		scanf("%d",&begin);
		if (begin==0) exit(0);
		sigprocmask(SIG_UNBLOCK, &mask, NULL);
		}
	else begin=2;
	
	/* Status Routine will handle timer and INTR */

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGALRM);
	sigaddset(&mask, SIGQUIT);
	action.sa_flags=0;
	action.sa_mask=mask;
	
	action.sa_handler=status;
	sigaction(SIGINT,&action,NULL);
	sigaction(SIGALRM,&action,NULL);

	
	action.sa_handler=query;
	sigaction(SIGQUIT,&action,NULL);

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

	alarm(0); /* shutoff alarm */

	if (signum == SIGINT) printf("Interrupt ");
	if (signum == SIGALRM) printf("Timer ");

	printf("processing %d\n",n);

	alarm(20);	/*restart alarm*/
}	

void query() {siglongjmp(jmpenv,1);}
