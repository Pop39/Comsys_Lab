#include    <sbuf.h>

//#define _USE_LOCK_

void sbuf_init( sbuf_t *sp , int length )
{
    sp->buffer = (int*)malloc( sizeof(int) * length );
    sp->max_length = length;
    sp->current = 0;
    sp->length = 0;
    sp->end = 0;
#ifdef _USE_LOCK_
    pthread_mutex_init( &(sp->mutex_length) , NULL );
#endif // _USE_LOCK_ 
} // function sbuf_init

void sbuf_deinit( sbuf_t *sp )
{
    free( sp->buffer );
} // function sbuf_deinit

void sbuf_in( sbuf_t *sp , int item , int thread_id )
{
    printf("Thread %d have put %d\n" , thread_id , item );
    (sp->buffer)[sp->end] = item;
    sp->end += 1;
    sp->end %= sp->max_length;
#ifdef _USE_LOCK_
    pthread_mutex_lock( &(sp->mutex_length ) );
#endif // _USE_LOCK_
    sp->length += 1;
#ifdef _USE_LOCK_
    pthread_mutex_unlock( &( sp->mutex_length ) );
#endif // _USE_LOCK_
} // function sbuf_in   

int sbuf_out( sbuf_t *sp , int thread_id )
{   
    int temp = (sp->buffer)[sp->current];
    printf("Thread %d have get %d\n" , thread_id , temp );
    sp->current += 1;
    sp->current %= sp->max_length;
#ifdef _USE_LOCK_
    pthread_mutex_lock( &(sp->mutex_length ) );
#endif // _USE_LOCK_
    sp->length -= 1;
#ifdef _USE_LOCK_
    pthread_mutex_unlock( &( sp->mutex_length ) );
#endif // _USE_LOCK_
    return temp;
} // function sbuf_out

int sbuf_length_reader( sbuf_t *sp )
{
    return sp->length;
} // function sbuf_length

int sbuf_length_writer(sbuf_t *sp )
{
    return sp->max_length - sp->length;
}
