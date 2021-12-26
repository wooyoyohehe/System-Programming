/* Program to illustrate message quees on System V */
/* Mary will receive half John's data in small messages */

#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define KEY (key_t)12345	/*key for message queue */

main() {

	int qid;	/* message queue id */
	int j;		/*loop counter */
	int sum;

	struct  {
		long type;
		int data;
		} my_msg;
		
			/* create queue if necessary */
	if ((qid=msgget(KEY,IPC_CREAT |0660))== -1) {
		perror("msgget");
		exit(1);
		}


			/* Now read the messages */

	j=0; sum=0;

	while (1) {
		msgrcv(qid,&my_msg,sizeof(my_msg.data),2,0);
		if (my_msg.data <0) break;
		sum +=  my_msg.data;
		j++;
		}
	

	printf("Got %d type %d messages from John adding up to %d\n",j,my_msg.type,sum);	
}
