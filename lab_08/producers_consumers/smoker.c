#include    "smoker.h"

void sbuf_init( sbuf_t* sp , int n )
{
    sp->buffer = (int*)malloc( sizeof( int ) * n * 2 );
    sp->max = n*2;
    sp->slots = (sem_t*)malloc( sizeof( sem_t) );
    sp->mutex = (sem_t*)malloc( sizeof( sem_t) );
    sp->table = (sem_t*)malloc( sizeof( sem_t) );
    sp->num_paper = 0;
    sp->num_tobacco = 0;
    sp->num_match = 0;
    sem_init( sp->slots , 0 , n );
    sem_init( sp->mutex , 0 , 1 );
    sem_init( sp->table , 0 , 0 );
    for( int run = 0 ; run < sp->max ; run++ )
    {
        sp->buffer[run] = 0 ;
    }
} // function sbuf_init

void sbuf_deinit( sbuf_t* sp )
{
    free( sp->buffer );    
} // function sbuf_deinit

void sbuf_insert( sbuf_t* sp , int first_item , int second_item , char* msg )
{
    for( int run = 0 ; run < sp->max ; run++ )
    {
        if( (sp->buffer)[run] == 0 )
        {
            (sp->buffer)[run] = first_item;
            switch ( first_item )
            {
                case 1 :
                    sp->num_paper += 1;
                    break;
                case 2 :
                    sp->num_tobacco += 1;
                    break;
                case 3 :
                    sp->num_match += 1;
                    break;
            }
            break;
        }    
    }
    for( int run = 0 ; run < sp->max ; run++ )
    {
        if( (sp->buffer)[run] == 0 )
        {
            (sp->buffer)[run] = second_item;
            switch ( second_item )
            {
                case 1 :
                    sp->num_paper += 1;
                    break;
                case 2 :
                    sp->num_tobacco += 1;
                    break;
                case 3 :
                    sp->num_match += 1;
                    break;
            }
            break;
        }    
    }
    printf("%s\n" , msg );
} // function sbuf_insert

void sbuf_remove( sbuf_t* sp , int first_item , int second_item , char* msg )
{
    for( int run = 0 ; run < sp->max ; run++ )
    {
        if( (sp->buffer)[run] == first_item )
        {
            (sp->buffer)[run] = 0;
            switch( first_item )
            {
                case 1 :
                    sp->num_paper -= 1;
                    break;
                case 2 :
                    sp->num_tobacco -= 1;
                    break;
                case 3 :
                    sp->num_match -= 1;
                    break;
            }
            break;
        }
    }
    for( int run = 0 ; run < sp->max ; run++ )
    {
        if( (sp->buffer)[run] == second_item )
        {
            (sp->buffer)[run] = 0;
            switch( second_item )
            {
                case 1 :
                    sp->num_paper -= 1;
                    break;
                case 2 :
                    sp->num_tobacco -= 1;
                    break;
                case 3 :
                    sp->num_match -= 1;
                    break;
            }
            break;
        }
    }
    printf("%s\n" , msg );
} // function sbuf_remove

int sbuf_look( sbuf_t * sp , int first_item )
{
    int result = 0;
    switch( first_item )
    {
        case 1 :
            if( sp->num_paper > 0 )
            {
                result = 1;
            }
            break;
        case 2 :
            if( sp->num_tobacco > 0 )
            {
                result = 1;
            }
            break;
        case 3 :
            if( sp->num_match > 0 )
            {
                result = 1;
            }
            break;
    }
    return result;
} // function sbuf_look one item
