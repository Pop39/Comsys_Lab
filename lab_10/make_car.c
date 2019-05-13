// FILE			: make_car.c
// AUTHOR		: K.Supasan , L.Panot
// CREATE ON	: 2019, May 13 (UTC+0)
// MAINTAINER	: K.Supasan

// MACRO DETAIL

// README
//  This file use process pattern follow make_cake.c
//  What that mean?
//      That mean in assembly line will have only object to process target
//  Follow problem we draw assembly line is
//
//                     +-----> A -----+
//  --->+----> B ------+-----> D -----+-----> D -----> A ---+
//      |              +-----> C -----+                     |
//      |                                                   |
//      +---------------------------------------------------+
//       FIRST          SECOND          THIRD    FORTH

// REFERENCE

// MACRO SET

#include    <stdio.h>
#include    <pthread.h>
#include    <unistd.h>

pthread_mutex_t lock;
pthread_cond_t firstOrder;
pthread_cond_t secondOrder;
pthread_cond_t thirdOrder;
pthread_cond_t forthOrder;

int numberCars = 1;
int putedChassis = 0;
int putedTries = 0;
int placedEngine = 0;
int attachedSeats = 0;
int assembledTopCover = 0;

void putsChassis();
void putsTires();
void placesEngine();
void attachesSeats();
void assemblesTopCover();
void paints();

void checkThirdOrder();
void sendCars();

void* personA( void *arg );
void* personB( void *arg );
void* personC( void *arg );
void* personD( void *arg );

int main( int argc , char* argv )
{
    pthread_mutex_init( &lock , 0 );
    pthread_cond_init( &firstOrder , 0 );
    pthread_cond_init( &secondOrder , 0 );
    pthread_cond_init( &thirdOrder , 0 );
    pthread_cond_init( &forthOrder , 0 );
    pthread_t threadA, threadB , threadC , threadD;
    pthread_create( &threadA , NULL , personA , NULL );
    pthread_create( &threadB , NULL , personB , NULL );
    pthread_create( &threadC , NULL , personC , NULL );
    pthread_create( &threadD , NULL , personD , NULL );
    while( 1 )
    {
        sleep( 1000 );
    }
    return 0;
}

void* personA( void *arg )
{
    pthread_mutex_lock( &lock ); // request lock
    while( 1 )
    {
        while( ( ! putedChassis ) || putedTries )
        {
            pthread_cond_wait( &secondOrder , &lock );
        } // wait for have putedChassis and didn't put tries
        putsTires();
        checkThirdOrder();
        // when I was putsTires() A have wait to paint
        while( ! assembledTopCover )
        {
            pthread_cond_wait( &forthOrder , &lock );
        }
        paints(); // when I paints finish we want to do next cars
        sendCars();
        pthread_cond_signal( &firstOrder ); 
    }
} // function personA

void* personB( void *arg )
{
    pthread_mutex_lock( &lock );
    while( 1 )
    {
        while( putedChassis ) // i have to putedChassis
        {
            pthread_cond_wait( &firstOrder , &lock );
        }
        putsChassis();
        pthread_cond_broadcast( &secondOrder );
    }
}

void* personC( void *arg )
{
    pthread_mutex_lock( &lock );
    while( 1 )
    {
        while( ( ! putedChassis ) || attachedSeats )
        {
            pthread_cond_wait( &secondOrder , &lock );
        }
        attachesSeats();
        checkThirdOrder();
    }
}

void* personD( void *arg )
{
    pthread_mutex_lock( &lock );
    while( 1 )
    {
        while( ( ! putedChassis ) || placedEngine )
        {
            pthread_cond_wait( &secondOrder , &lock );
        }
        placesEngine();
        while( ! ( placedEngine && attachedSeats && putedTries ) )
        {
            pthread_cond_wait( &thirdOrder , &lock );
        }
        assemblesTopCover();
        pthread_cond_signal( &forthOrder );
    }
}

void checkThirdOrder()
{
    if( attachedSeats && placedEngine && putedTries )
    {
        pthread_cond_signal( &thirdOrder );
    }
}

void putsChassis()
{
    printf( "Car #%d\n" , numberCars );
    printf( "B puts chassis on the conveyor\n" );
    putedChassis = 1; 
}

void attachesSeats()
{
    printf( "C attached seats\n" );
    attachedSeats = 1;
}

void placesEngine()
{
    printf( "D places engine\n" );
    placedEngine = 1;
}

void putsTires()
{
    printf( "A puts tires\n");
    putedTries = 1;
}

void assemblesTopCover()
{
    putedChassis = 0;
    putedTries = 0;
    placedEngine = 0;
    attachedSeats = 0;
    printf( "D assembles top cover\n" );
    assembledTopCover = 1;
}

void paints()
{
    assembledTopCover = 0;
    printf( "A paints\n" );
}

void sendCars()
{
    sleep(1);
    numberCars++;
}
