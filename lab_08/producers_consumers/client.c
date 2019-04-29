#include    <stdio.h> // standard library of c language
#include    <stdlib.h> 
#include    <unistd.h>
#include    <pthread.h>
#include    <semaphore.h>
#include    "smoker.h"

//#define _PRINT_DATA_
//#define _LOCK_PROCESS_

int length_data; // use to assing order put item
sbuf_t* problem_c_and_s;

const static unsigned int time_sleep = 1 ;

void *agent( void *arg )
{
    int* list_data = (int*) malloc( sizeof( int )* length_data );
    list_data = (int*) arg;
#ifdef _PRINT_DATA_
    printf( "Agent have %d item" , length_data );
    for( int run = 0 ; run < length_data ; run++ )
    {
        printf( " %d" , list_data[run] );
    }
    printf( "\n");
#endif
    int result_item , run ;
    char* my_message;
    char first_message[] = "Agent places tobacco and match";
    char second_message[] = "Agent places paper and match";
    char third_message[] = "Agent places paper and tobacco";
#ifdef _PRINT_DATA_
        printf("Loop semaphor of agent\n");
#endif
    for( run = 0 ; ; )
    {
#ifdef _PRINT_DATA_
        printf("Try to find data\n");
#endif
        run = run % length_data;
        result_item = list_data[run] + list_data[run+1];
#ifdef _PRINT_DATA_
        printf("Result item is %d\n" , result_item );
#endif
        switch( result_item )
        {
            case 3 :
                my_message = &first_message[0];
                break;
            case 4 :
                my_message = &second_message[0];
                break;
            case 5 :
                my_message = &third_message[0];
                break;
        }
        sem_wait( problem_c_and_s->slots );
        // I can put item next lock buffer
        sem_wait( problem_c_and_s->mutex );
#ifdef _PRINT_DATA_
        printf("Agent insert item\n");
#endif
        sbuf_insert( problem_c_and_s , list_data[ run ] , list_data[run+1] , my_message );
#ifdef _LOCK_PROCESS_
        printf("After insert numer paper , tobacco and match : %d , %d , %d\n" ,
                problem_c_and_s->num_paper , problem_c_and_s->num_tobacco 
                , problem_c_and_s->num_match );
#endif 
        sem_post( problem_c_and_s->mutex );
#ifdef _LOCK_PROCESS_
        printf("Agent open table\n");
#endif
        sem_post( problem_c_and_s->table ); // have add part of item
        run += 2;
    }
} // function agent

void *smoker( void *arg )
{
    int my_item = (int*) arg ;
#ifdef _PRINT_DATA_
    printf("I'm smoker my item is %d\n" , my_item );
#endif
    int first_item = 0;
    int second_item = 0;
    char* my_message;
    char first_message[] = "Paper smoker smokes";
    char second_message[] = "Tobacco smoker smokes";
    char third_message[] = "Match smoker smokes";
#ifdef _PRINT_DATA_
    printf("Smoke assign type\n" );
#endif
    switch ( my_item )
    {
        case 1 :
            first_item = 2;
            second_item = 3;
            my_message = &first_message[0];
            break;
        case 2 :
            first_item = 3;
            second_item = 1;
            my_message = &second_message[0];
            break;
        case 3 :
            first_item = 1;
            second_item = 2;
            my_message = &third_message[0];
            break;
    }
    int can_smoke; 
    while( 1 )
    {
        nanosleep( &tim , &tim2 );
        sem_wait( problem_c_and_s->table );
#ifdef _LOCK_PROCESS_
        printf("Smoker type %d lock table\n" , my_item );
#endif
        sem_wait( problem_c_and_s->mutex );
#ifdef _LOCK_PROCESS_
        printf("Smoker type %d lock mutex\n" , my_item );
#endif
        can_smoke = sbuf_look( problem_c_and_s , first_item ) 
                && sbuf_look( problem_c_and_s , second_item );
        if( can_smoke == 1)
        {
            sbuf_remove( problem_c_and_s , first_item , second_item , my_message );
#ifdef _LOCK_PROCESS_
        printf("After smoke numer paper , tobacco and match : %d , %d , %d\n" ,
                problem_c_and_s->num_paper , problem_c_and_s->num_tobacco 
                , problem_c_and_s->num_match );
#endif 
        }
        sem_post( problem_c_and_s->mutex );
        // smoking
        if( can_smoke == 0 )
        {
            sem_post( problem_c_and_s->table );
#ifdef _LOCK_PROCESS_
            printf("Smoker type %d can not smoke\n" , my_item );
#endif
            continue;
        }
        else
        {
            nanosleep( &tim , &tim2 );
            // get pair items 
#ifdef _LOCK_PROCESS_
            printf("Smoker type %d unlock slots\n" , my_item );
#endif
            sem_post( problem_c_and_s->slots );
        }
    } 

} // function smoker

int convert( char* char_data )
{
    char temp = char_data[0];
    return temp - '0';
} // function num_char

int main( int argv , char** argc )
{
    int* list_data;
    tim.tv_sec = 0;
    tim.tv_nsec = 1000000000 / 4 ;
    length_data = argv-1;
#ifdef _PRINT_DATA_
    printf("Number length_data is %d\n" , length_data );
#endif
    if( length_data == 0 )
    {
#ifdef _PRINT_DATA_
        printf("Case not have input\n");
#endif
        list_data = (int*)malloc( sizeof( int )*10 );
        length_data = 10;
        int temp[10] = { 1 , 2 , 3 , 1 , 2 , 3 , 2 , 1 , 3 , 1 };
        for( int run = 0 ; run < length_data ; run++ )
        {
            list_data[run] = temp[run];
        }
    }
    else
    {
#ifdef _PRINT_DATA_
        printf("Case have input\n");
#endif
        int temp[length_data];
        list_data = (int*)malloc( sizeof( int )*length_data );
        for( int run = 1 ; run <= length_data ; run++ )
        {
            temp[ run - 1] = convert( argc[run] );
        }
        for( int run = 0 ; run < length_data ; run++ )
        {
            list_data[run] = temp[run];
        }
    }

#ifdef _PRINT_DATA_
    printf( "List item is");
    for( int run = 0 ; run < length_data ; run++ )
    {
        printf( " %d" , list_data[run] );
    }
    printf( "\n");
#endif

    pthread_t tid[4];

    problem_c_and_s = ( sbuf_t* )malloc( sizeof( sbuf_t) );

    sbuf_init( problem_c_and_s , 1 );

#ifdef _PRINT_DATA_
    printf("Create Thread \n" );
#endif
    pthread_create( &(tid[0]) , NULL , agent , (void*) list_data );
    pthread_create( &(tid[1]) , NULL , smoker , (void*) 1 );
    pthread_create( &(tid[2]) , NULL , smoker , (void*) 2 );
    pthread_create( &(tid[3]) , NULL , smoker , (void*) 3 );

#ifdef _PRINT_DATA_
    printf("Finish create Thread \n" );
#endif

    pthread_exit( NULL );

}
