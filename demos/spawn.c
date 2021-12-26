/* program to illustrate process creation via fork
	and exec */
#include <stdio.h>
#include <sys/file.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>



char parent[15]; 	/* room for parents pid in ascii */
char *child[]={"myname=john","myname=mary"};   /* child's name */

int main(int argc, char *argv[], char *envp[])

{
	int i; /* generic counter */
	int pid[2];				/*pids of children */
	int whom;				/* pid of dead child*/
	int esize;				/* number of env variables */
	char **newenvp;         /* constructed env pointer */
	int status;				/* childs return status*/

	/* for fun we will build a new environment with
		some extra values */

	sprintf(parent,"parent=%d",getpid());

		/* how big is the old environment */

	for (esize=0; envp[esize] != NULL; esize++);

		/* make new envp 2 slots longer */

	newenvp=( char **) malloc((esize+3)*sizeof(char *));
	newenvp[0]=parent;
	/* newenvp[1] will be set differently for each child */
	for (i=0; i<=esize; i++)
		newenvp[i+2]=envp[i];

	for (i=0; i<2; i++) {

		if ((pid[i]=fork()) == -1) {
			perror("fork");
			exit (i+3);
			}
		
		if (pid[i] == 0) {
			/* this is the child */
			newenvp[1]=child[i];  /* tell him his name */
			sleep(getpid()%4); /* random wait for some drama*/

			/* we will redirect stdout */
			close(1);
			if (open((i ? "mary" : "john"),O_WRONLY |O_CREAT |O_TRUNC,0644)
				== -1) {
					perror("open stdout");
					exit (2*(1+i));
					}

			/* now exec the new program */
			execle("prenv","prenv",NULL,newenvp);
			perror("execle"); /* shoudn't get here */
			exit(4*(i+1));
			}
		}

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
