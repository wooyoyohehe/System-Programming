
/* program to illustrate receiving half of fifo on System V */
		

#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {


	int fifofd;		/*file descriptor for fifo */
	int j;			/*Loop counter */
	int sum;		/* sum of items sent */
	int n;			/*input buffer */

	j=1;

	/* make the fifo - ignore error if it exists */

	if ((mkfifo("/tmp/john",0666)== -1) &&
		errno!= EEXIST) {
		perror("mkfifo");
		exit(1);
		}

		/* open fifo with O_NONBLOCK so we won't block forever */
	while ((fifofd=open("/tmp/john",O_RDONLY |O_NONBLOCK)) == -1) {
			perror("fifo open");
			exit(2);
		}


			/* John will generate numbers and Mary will add them*/

			/* read one number without blocking */
	 while (read(fifofd,&sum,sizeof (int))==0) {
			printf("WHERE is that JOHN\n");
			sleep(20);
			}
	 fcntl(fifofd,F_SETFL,O_RDONLY); /* cancel out O_NOBLOCK */

			/*now read the rest */
	while (read(fifofd, &n,sizeof (int))!= 0){
			sum+=n;
			j++;
			}
	close(fifofd); /* close the output fifo*/

	printf("I Mary read %d numbers totaling %d\n",j,sum);


}
