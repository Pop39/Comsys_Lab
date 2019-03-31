#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <sys/wait.h>
#include    <fcntl.h> // library for import system call

int main( void )
{
    int fd;
    fd = open("output.txt" , O_CREAT|O_TRUNC|O_WRONLY , 0666 );
    if( !fork() )
    {
        write( fd , "hello " , 6);
        exit(0);
    }
    else
    {
        int status;
        wait(&status);
        write( fd , "world\n" , 6);
    }
}
