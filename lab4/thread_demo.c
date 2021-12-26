#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>

int x = 0;
int num_of_1 = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int iter;

void* produce()
{
    for(int i = 0; i<iter; i++) 
    {
        pthread_mutex_lock(&mutex);
        printf("I'm producing the food\n");
        printf("The food is %d\n", x);
        x++;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        sleep(1);
    }

}

void* consume()
{
    pthread_mutex_lock(&mutex);
    while(x<=0)
    {
        printf("No food, waiting for producing\n");
        pthread_cond_wait(&cond, &mutex);
    }
    x-=5;
    pthread_mutex_unlock(&mutex);
    
}


int main(int argc, char *argv[])
{
    iter = atol(argv[1]);
    int thread_num = atol(argv[2]);
    pthread_t threads[thread_num];

    if(pthread_create(&threads[0], NULL, &produce, NULL)!=0)
    {
        perror("Failed\n");
    }

    if(pthread_create(&threads[1], NULL, &consume, NULL)!=0)
    {
        perror("Failed\n");
    }
    
    for(int i=0; i<thread_num; i++)
    {
        if(pthread_join(threads[i], NULL)!=0)
        {
            perror("Failed\n");
        }
    }




    return 0;
}