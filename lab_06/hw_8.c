#include    <stdio.h>
#include    <stdlib.h>
#include    <pthread.h> // import pthread_t
#include    <unistd.h> // import getpid

int global_number = 0;

void* task( void* arg )
{
    pid_t pid = getpid();
    printf("My pid is %d and global number %d\n" , pid , global_number );
    global_number++;
    return NULL;
}

int main(){
    pthread_t thread;
    pthread_create(&thread , NULL , &task , NULL );
    global_number = 3;
    task( NULL );
    global_number = 6;
    return 0;
}
