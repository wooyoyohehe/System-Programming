
/* Program to illustrate sharing memory on System V */
/* Mary will read a vector of numbers in shared memory */

#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define KEY (key_t)12347	/*key for shared memory segment */

int main() {

	int sid;	/* segment id of shared memory segment */
	int *array;	/* pointer to shared array, no storage yet*/
	int j;		/*loop counter */
	int sum; 	/*running sum*/
		

			/* create shared segment if necessary */
	if ((sid=shmget(KEY,100*sizeof (int),IPC_CREAT |0660))== -1) {
		perror("shmget");
		exit(1);
		}

			/* map it into our address space*/

	if ((array=((int *) shmat(sid,0,0)))== (int *)-1) {
		perror("shmat");
		exit(2);
		}

			/* Now add it up */

	sum=0;
	for (j=0;j<=100;j++) sum+=array[j];

	printf("Mary says array sum is %d\n",sum);

	/* Unmap and deallocate the shared segment */

	if (shmdt((char  *) array) == -1) {
		perror("shmdt");
		exit(3);
	}
	if (shmctl(sid,IPC_RMID,0) == -1) {
		perror("shmctl");
		exit(3);
	}
}
