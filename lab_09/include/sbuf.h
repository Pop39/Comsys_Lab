#include    <stdio.h>
#include    <stdlib.h>

#include    <pthread.h>

#ifndef __SBUF_H__
#define __SBUF_H__

static struct timespec tim , tim2;

typedef struct
{
    int* buffer; //  buffer of data
    int max_length; //  length of buffer
    int current; // point to order in array
    int end; // point to end data now
    int length; 
    pthread_mutex_t mutex_length;
} sbuf_t;

// buffor style FIFO

void sbuf_init( sbuf_t *sp , int length ); // init one sbuf_t
void sbuf_deinit( sbuf_t *sp ); // free buffer
void sbuf_in( sbuf_t *sp , int item , int thread_id ); //  append data to array
int sbuf_out( sbuf_t *sp , int thread_id );
int sbuf_length_writer( sbuf_t *sp ); // return lenght of empty slots
int sbuf_length_reader( sbuf_t *sp ); // return lenght of data slots

#endif // __SBUF_H__
