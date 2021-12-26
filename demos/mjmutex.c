#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void *producer(),*consumer();

int *array;
int limit=-1;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

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

	for (i=0;i<*howmany;i++) {
		array[i]=i;
		if (!(i%1000)) { 
			pthread_mutex_lock(&mtx);
			limit=i;
			pthread_mutex_unlock(&mtx);
		}
	}
	pthread_mutex_lock(&mtx);
		limit=i-1;
	pthread_mutex_unlock(&mtx);

	printf("John produced %d Numbers\n",*howmany); 
	pthread_exit(NULL);
}


void *consumer(int *howmany) {
	int i,mylimit=0;
	long long sum=0;
	long long *wait; 
	wait =malloc (sizeof (long long));
	*wait=0;

	i=0;
	while (i< *howmany)	{
		pthread_mutex_lock(&mtx);
		if (mylimit >= limit) (*wait)++;
		mylimit=limit;
		pthread_mutex_unlock(&mtx);
		
		while (i<=mylimit) sum+=array[i++];
		}

	printf("Mary consumed %d Numbers for a total of %lld\n",*howmany,sum); 
	pthread_exit(wait);
}
