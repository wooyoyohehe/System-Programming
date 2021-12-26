#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void *producer(),*consumer();

int *array;

int main(int argc, char **argv) {

	 int howmany;
	 pthread_t johntid,marytid;
	 long long *howlong;

	 howmany=atoi(argv[1]);

	 array=calloc(howmany,sizeof(int));

	 pthread_create(&marytid,NULL, consumer, &howmany);
	 pthread_create(&johntid,NULL, producer, &howmany);
	 pthread_join(johntid,NULL);
	 pthread_join(marytid,(void **)&howlong);

	 printf("John and Mary Threads done with wait %lld\n",*howlong);

}

void * producer(int *howmany) {
	int i;

sleep(1);
	for (i=0;i<*howmany;i++) array[i]=i+1;
	printf("John produced %d Numbers\n",*howmany); 
	pthread_exit(NULL);
}


void *consumer(int *howmany) {
	int i;
	long long sum=0;
	long long *wait; 
	wait =malloc (sizeof (long long));
	*wait=0;

	for (i=0;i<*howmany;i++) {while (!array[i]) (*wait)++; sum+=array[i];}
	printf("Mary consumed %d Numbers for a total of %lld\n",*howmany,sum); 
	pthread_exit(wait);
}
