#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "barrier.h"

int limit = 0;

pthread_mutex_t mutex;
pthread_cond_t condvar;
int arrived = 0;

int barrier_init(int n)
{
    limit = n;
    arrived = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condvar, NULL);
}

int wait_for_others(void)
{

    // complete the synchronziation logic here. 
    pthread_mutex_lock(&mutex);
    arrived++;
    while(arrived != limit){
        pthread_cond_wait(&condvar, &mutex);  //wait for others
    }
    pthread_cond_signal(&condvar); //eentje (= laatste) geraakt erdoor en laat de rest los
    pthread_mutex_unluck(&mutex);
    return 0;
}
