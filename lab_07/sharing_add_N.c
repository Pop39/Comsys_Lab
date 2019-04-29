/* $begin sharing */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 100
void *thread(void *vargp);

char **ptr; /* Global variable */ //line:conc:sharing:ptrdec

int main()
{
    int i;
    pthread_t tid;
    char *msgs[N] = {
        "Hello from 0"
        ,"Hello from 1"
        ,"Hello from 2"
        ,"Hello from 3"
        ,"Hello from 4"
        ,"Hello from 5"
        ,"Hello from 6"
        ,"Hello from 7"
        ,"Hello from 8"
        ,"Hello from 9"};

    ptr = msgs;
    for (i = 0; i < N; i++)
        pthread_create(&tid, NULL, thread, (void *)i);
    pthread_exit(NULL);
}

void *thread(void *vargp)
{
    int myid = (int)vargp;
    static int cnt = 0;                                    //line:conc:sharing:cntdec
    printf("[%d]: %s (cnt=%d)\n", myid, ptr[myid%10], ++cnt); //line:conc:sharing:stack
    return NULL;
}
/* $end sharing */
