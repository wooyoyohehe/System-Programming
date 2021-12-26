/* program to illustrate process creation via fork
	    and exec */

#include <sys/types.h> 
#include <fcntl.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <errno.h>
#include <sys/wait.h>

char parent[10]; 	/* room for parents pid in ascii */
char *child[]={"myname=john","myname=mary"};   /* child's name */

int main(int argc, char **argv, char **envp)

{
	int i; /* generic counter */
	int pid[2];				/*pids of children */
	int whom;				/* pid of dead child*/
	int esize;				/* number of env variables */
	char **newenvp;         /* constructed env pointer */
	int status;				/* childs return status*/
	int pipefd[2];			/* fds for ends of pipe */

	/* for fun we will build a new environment with
		some extra values */

	sprintf(parent,"parent=%d",getpid());

		/* how big is the old environment */

	for (esize=0; envp[esize] != NULL; esize++);

		/* make new envp 2 slots longer */

	newenvp=( char **) malloc((esize+3)*sizeof(char *));
	newenvp[0]=parent;
	/* newenvp[1] will be set differently for each child */
	for (i=0; i<esize; i++)
		newenvp[i+2]=envp[i];

	/* before forking set up a pipe */
		if (pipe(pipefd) == -1) {
			perror("pipe");
			exit(10);
			}

	for (i=0; i<2; i++) {

		if ((pid[i]=fork()) == -1) {
			perror("fork");
			exit (i+1);
			}
		
		if (pid[i] == 0) {
			/* this is the child */
			newenvp[1]=child[i];  /* tell him his name */
			sleep(getpid() %4); /* random wait for some drama*/

			/* we will redirect stdout */
			close(1);
			if (open((i ? "mary" : "john"),O_WRONLY |O_CREAT |O_TRUNC,0644)
				== -1) {
					perror("open stdout");
					exit (2*(1+i));
					}

			/* close the side of pipe I am not going to use */
			close (pipefd[i]);

			/* John will generate numbers and Mary will add them*/
			if (i==0) { /* john does this */
				int	j;
				for (j=1;j<=10;j++) 
					write(pipefd[1], &j,sizeof (int));
				close(pipefd[1]); /* close the output pipe*/
				printf("I wrote 10 number to Mary\n");
				}

			if (i==1) { /* mary does this */
				int k, sum;
				sum=0;
				while (read (pipefd[0], &k, sizeof (int)))
					sum += k;
				printf("The sum of John's numbers is %d\n",sum);
				}
			exit (0); /* either way exit */
			}
		}

			/* parent must close pipe or Mary will not see eof */
			close(pipefd[0]); close(pipefd[1]);

			/* now parent waits */

			for (i=0; i<=1; i++ ) {

				if ((whom=wait(&status)) == -1) {
					perror("wait");
					exit(1);
					}
				
				if (whom == pid[0] )
					printf("John died with status=%x\n",status);
				else if (whom == pid[1] )
					printf("Alas poor Mary died with status=%x\n",status);
				else
					printf("Beats me who died, his pid is=%d",whom);

				}
}



