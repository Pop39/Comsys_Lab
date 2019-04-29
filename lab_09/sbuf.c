#include    <sbuf_h>

void sbuf_init( sbuf_t *sp , int length )
{
    sp->buffer = (int*)malloc( sizeof(int) * length );
    sp->max_length = length;
    sp->current_length = 0;
    sp->current = 0;
    sp->end = 0;
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
} // function sbuf_in   

int sbuf_out( sbuf_t *sp , int thread_id )
{   
    int temp = (sp->buffer)[sp->current];
    printf("Thread %d have get %d\n" , thread_id , temp );
    sp->current += 1;
    sp->current %= sp->max_length;
    return temp;
} // function sbuf_out

int sbuf_length( sbuf_t *sp )
{
    int temp = sp->end - sp->current;
    if( temp < 0 )
    {
        temp += sp->max_length;
    }
    return temp;
} // function sbuf_length
