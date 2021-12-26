
/* Program to illustrate sharing memory on System V */
/* Mary will read a vector of numbers in shared memory */
/* Also uses a semaphore to lock the region from Mary
	until John is done with it. Note this is only
	a one way lock.*/

#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#define KEY (key_t)12346	/*key for shared memory segment */

	struct sembuf sb;	/* semaphore buffer */

int main() {

	int sid;	/* segment id of shared memory segment */
	int *array;	/* pointer to shared array, no storage yet*/
	int j;		/*loop counter */
	int sum; 	/*running sum*/
	int semid;	/* semaphore id */
		

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

			/* get semaphore id*/
	if ((semid=semget(KEY,1 ,IPC_CREAT |0660))== -1) {
		perror("semget");
		exit(1);
		}
	
	sb.sem_op =-1;  /* set up for a lock operation*/
	sb.sem_num =0;
	sb.sem_flg =0;

	if (semop(semid, &sb, 1) == -1) { /* will block if locked */
		perror("sem lock");
		exit(1);
		}

			/* Now add it up */

	sum=0;
	for (j=0;j<=100;j++) sum+=array[j];

	printf("Mary says arrary sun is %d\n",sum);

	/* Unmap and deallocate the shared segment */

	if (shmdt( (char *) array) == -1) {
		perror("shmdt");
		exit(3);
	}
	if (shmctl(sid,IPC_RMID,0) == -1) {
		perror("shmctl");
		exit(3);
	}

}
