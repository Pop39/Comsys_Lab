#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <sys/wait.h>

int main( void )
{
    pid_t pid = fork();
    int exit;
    if( pid != 0 )
    {
        printf( "When pid != 0 data of exit before is %d\n" , exit );
        wait(&exit);
        printf( "When pid != 0 data of exit after is %d\n" , exit );
    }
    printf("Hello World\n: %d\n",pid);
}
