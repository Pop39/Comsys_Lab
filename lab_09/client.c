#include    <stdio.h>
#include    <pthread.h>
#include    <sbuf.h>

#define _PRINT_PRODUCER_

#define _PRINT_PROCESS_

int id_consumer = 100;
int id_producer = 200;

pthread_mutex_t mutex_reader = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_reader = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_writer = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_writer = PTHREAD_COND_INITIALIZER;

sbuf_t* data;

void* consumer_thread()
{
    int my_id = 0 ;
    pthread_mutex_lock( &mutex_reader );
    my_id = id_consumer;
    id_consumer++;
    pthread_mutex_unlock( &mutex_reader );
   
    int my_item; 
    int temp;
    while( 1 )
    {
        pthread_mutex_lock( &mutex_reader );
        while( 1 ){
            temp = sbuf_length_reader( data );
            if( temp < 0 )
            {
                pthread_cond_wait( &cond_reader , &mutex_reader );
            }
            else
            {
                break;
            }
        }
        my_item = sbuf_out( data , my_id);
        pthread_mutex_unlock( &mutex_reader );
        pthread_cond_signal( &cond_writer );
        nanosleep( &tim , &tim2 );
    }
} // function consumer_thread

void* producer_thread( void* list_data )
{
    int* my_data = (int*)list_data;
    pthread_mutex_lock( &mutex_writer );
    int my_id = id_producer;
    id_producer++;
    pthread_mutex_unlock( &mutex_writer );

#ifdef _PRINT_PRODUCER_
    pthread_mutex_lock( &mutex_writer );
    printf( "My id is %d and have data" , my_id );
    for( int run = 0 ; run < 5 ; run++ )
    {
        printf( "%4d" , my_data[run] );
    }
    printf("\n");
    pthread_mutex_unlock( &mutex_writer );
#endif

    int run = 0 ;
    int temp;
    while( 1 )
    {
        pthread_mutex_lock( &mutex_writer );
        while( 1 ){
            temp = sbuf_length_writer( data );
            if( temp < 0 )
            {
                pthread_cond_wait( &cond_writer , &mutex_writer );
            }
            else
            {
                break;
            }
        }
        sbuf_in( data , my_data[run] , my_id);
        pthread_mutex_unlock( &mutex_writer );
        pthread_cond_signal( &cond_reader );
        run++;
        run %= 5;
        nanosleep( &tim , &tim2 );
    }
}

int main( int argc , char** argv )
{

    tim.tv_sec = 0;
    tim.tv_nsec = 1000000000 / 2 ;

    pthread_t consumer[2];
    pthread_t producer[2];

#ifdef _PRINT_PROCESS_
    printf("Finish declare pthread_t\n");
#endif

    data = (sbuf_t*) malloc( sizeof( sbuf_t ) );

    sbuf_init( data , 5 );

#ifdef _PRINT_PROCESS_
    printf("Finish declare sbut_init\n");
#endif

    int first_set_data[ 5 ] = { 1 , 2 , 3 , 4 , 5 };
    int second_set_data[ 5 ] = { -1 , -2 , -3 , -4 , -5 };

#ifdef _PRINT_PROCESS_
    printf("Finish declare producer_data\n");
#endif

    pthread_create( &(consumer[0] ) , NULL , consumer_thread , NULL );
    pthread_create( &(consumer[1] ) , NULL , consumer_thread , NULL );

    pthread_create( &(producer[0] ) , NULL , producer_thread , first_set_data );
    pthread_create( &(producer[1] ) , NULL , producer_thread , second_set_data );

    pthread_join( consumer[0] , NULL );
    pthread_join( consumer[1] , NULL );

    pthread_join( producer[0] , NULL );
    pthread_join( producer[1] , NULL );

    sbuf_deinit( data );

    return 0;
}
