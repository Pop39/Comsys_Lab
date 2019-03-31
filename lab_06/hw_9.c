#define _OPEN_THREADS

#include    <stdio.h>
#include    <stdlib.h>
#include    <pthread.h> // import pthread_t
#include    <unistd.h> // import getpid

void *worker( void* argc )
{
    printf( "Worker and our thread is %ld\n" , pthread_self() );
    return NULL;
}

int main()
{
    pthread_t thread;
    pthread_create( &thread , NULL , &worker , NULL );
    pthread_yield( NULL );
    printf("Main and return_value is %ld\n" , pthread_self() );
    return 0;
}
