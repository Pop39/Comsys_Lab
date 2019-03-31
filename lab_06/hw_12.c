#define _OPEN_THREADS

#include    <stdio.h>
#include    <stdlib.h>
#include    <pthread.h> // import pthread_t
#include    <unistd.h> // import getpid
#include    <sys/wait.h> // wait function

void *worker( void* arg )
{
    int* my_var = (int*) arg;
    *my_var = *my_var + 1;
    printf("My_var is %d\n" , *my_var );
    return( void*) 42;
}

int my_var;

int main()
{
    int status;
    my_var = 0;
    pthread_t  thread;
    pid_t pid = fork();
    if( pid == 0 )
    {
        pthread_create( &thread , NULL , &worker , &my_var );
        pthread_join( thread , NULL );
    }
    else
    {
        pthread_create( &thread , NULL , &worker , &my_var );
        pthread_join( thread , NULL );
        pthread_create( &thread , NULL , &worker , &my_var );
        pthread_join( thread , NULL );
        wait( &status );
    }
    return 0;
}
