/* 
 * goodcnt.c - A correctly synchronized counter program 
 */
/* $begin goodcnt */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

#include "common_threads.h"

void *thread(void *vargp); /* Thread routine prototype */

/* Global shared variables */
/* $begin goodcntsemdef */
volatile long cnt = 0; /* Counter */

#ifdef __APPLE__
dispatch_semaphore_t mutex;          /* Semaphore that protects counter */
#else
sem_t mutex; 
#endif
/* $end goodcntsemdef */

int main(int argc, char **argv)
{
    int niters;
    pthread_t tid1, tid2;

    /* Check input argument */
    if (argc != 2)
    {
        printf("usage: %s <niters>\n", argv[0]);
        exit(0);
    }
    niters = atoi(argv[1]);

    /* Create threads and wait for them to finish */
    /* $begin goodcntseminit */
    #ifdef __APPLE__
    mutex = dispatch_semaphore_create(1); // init with value of 1
    #else
    Sem_init(&mutex, 0, 1); /* mutex = 1 */
                            /* $end goodcntseminit */
    #endif
    Pthread_create(&tid1, NULL, thread, &niters);
    Pthread_create(&tid2, NULL, thread, &niters);
    Pthread_join(tid1, NULL);
    Pthread_join(tid2, NULL);

    /* Check result */
    if (cnt != (2 * niters))
        printf("BOOM! cnt=%ld\n", cnt);
    else
        printf("OK cnt=%ld\n", cnt);
    exit(0);
}

/* Thread routine */
void *thread(void *vargp)
{
    int i, niters = *((int *)vargp);
    static int numt = 0; 

    printf("thread num = %d doing %d iterations\n", ++numt, niters);

    /* $begin goodcntthread */
    for (i = 0; i < niters; i++)
    {
        #ifdef __APPLE__
        dispatch_semaphore_wait(mutex, DISPATCH_TIME_FOREVER);
        #else
        P(&mutex);
        #endif
        cnt++;
        #ifdef __APPLE__
        dispatch_semaphore_signal(mutex);
        #else
        V(&mutex);
        #endif
    }
    /* $end goodcntthread */
    return NULL;
}
/* $end goodcnt */
