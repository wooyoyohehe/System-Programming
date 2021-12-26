#include <stdio.h>
#include <pthread.h>

void* thread_test(void *t1){
    int tid = *((int*)(t1));
    printf("%d \n", tid);
}

int main(int argc, char* argv[])
{
    int indexes[2];
    indexes[0] = 1;
    indexes[1] = 2;

    pthread_t t1, t2;
    pthread_create(&t1, NULL, &thread_test, (void*)&indexes[0]);
    pthread_create(&t2, NULL, &thread_test, (void*)&indexes[1]);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

}