#ifndef _SMOKER_H__
#define _SMOKER_H__

#include    <stdio.h>
#include    <stdlib.h>
#include    <semaphore.h>

typedef struct
{
    int *buffer; // Buffer array
    int max; // length of buffer
    int num_paper; // number of paper in current buffer  -> 1
    int num_tobacco; // number of tabacco in current buffer -> 2
    int num_match; // number of match in current buffer -> 3
    sem_t *slots;
    sem_t *mutex;
    sem_t *table;
} sbuf_t;

static struct timespec tim , tim2;


void sbuf_init( sbuf_t *sp, int n ); // init structor
void sbuf_deinit( sbuf_t *sp );
int sbuf_look( sbuf_t *sp , int first_item );
void sbuf_insert( sbuf_t *sp , int first_item , int second_item , char *msg );
void sbuf_remove( sbuf_t *sp , int first_item , int second_item , char *msg );
#endif // _SMOKER_H__
