#include    <stdio.h>
#include    <stdlib.h>

#ifndef __SBUF_H__
#define __SBUF_H__

typedef struct
{
    int* buffer; //  buffer of data
    int max_length; //  length of buffer
    int current_length;
    int current; // point to order in array
    int end; // point to end data now 
} sbuf_t;

// buffor style FIFO

void sbuf_init( sbuf_t *sp , int length ); // init one sbuf_t
void sbuf_deinit( sbuf_t *sp ); // free buffer
void sbuf_in( sbuf_t *sp , int item , int thread_id ); //  append data to array
int sbuf_out( sbuf_t *sp , int thread_id );
int sbuf_length( sbuf_t *sp );

#endif // __SBUF_H__
