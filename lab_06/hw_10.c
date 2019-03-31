#define _OPEN_THREADS

#include    <stdio.h>
#include    <stdlib.h>
#include    <pthread.h> // import pthread_t
#include    <unistd.h> // import getpid

void *worker( void* argc )
{
    int *loc_var = (int*) argc;
    printf( "worker\n" );
    *loc_var = 2;
    return NULL;
}

int main()
{
    int i = 0;
    pthread_t thread;
    pthread_create( &thread , NULL , &worker , &i );
    pthread_join( thread , NULL );
    printf("i is %d\n" , i );
    return 0;
}
