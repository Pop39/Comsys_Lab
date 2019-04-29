#include "ex_02_account.h"

static struct timespec tim, tim2 ;

void account_init(account_info *sp, int n)
{
    int i;
        
    tim.tv_sec = 0;
    tim.tv_nsec = 100;
    for (i=0; i<n; i++) {
        sprintf(sp[i].name, "%s%d", "test_account", i);
        sp[i].id = i;
        sp[i].balance = INT_BALANCE;

        sp[i].ptr_write_sem = (sem_t*)malloc( sizeof( sem_t ) );
        sp[i].ptr_read_sem = (sem_t*)malloc( sizeof( sem_t ) );
        sem_init( sp[i].ptr_write_sem  , 0 , 1 );
        sem_init( sp[i].ptr_read_sem  , 0 , 2 );
    }
}

// Use only one semaphore to manage that

void account_deposit(account_info *sp, int account_id, int amount)
{
    int temp1;

    sem_wait( sp[account_id].ptr_read_sem );    
    temp1 = sp[account_id].balance;
    sem_post( sp[account_id].ptr_read_sem );    

    nanosleep(&tim, &tim2);

    sem_wait( sp[account_id].ptr_write_sem );
    account_writing( sp , account_id , amount );
    sem_post( sp[account_id].ptr_write_sem );
}

void account_withdraw(account_info *sp, int account_id, int amount)
{
    int temp1;

    do{
        sem_wait( sp[account_id].ptr_read_sem );
        temp1 = sp[account_id].balance;
        sem_post( sp[account_id].ptr_read_sem );
    }while( temp1 < amount );

    sem_wait( sp[account_id].ptr_write_sem );
    account_writing( sp , account_id , amount*-1 );
    sem_post( sp[account_id].ptr_write_sem );
}

void account_writing( account_info *sp , int account_id , int result )
{
    sem_wait( sp[account_id].ptr_read_sem );
    sem_wait( sp[account_id].ptr_read_sem );
    sp[account_id].balance += result;
    sem_post( sp[account_id].ptr_read_sem );
    sem_post( sp[account_id].ptr_read_sem );
}
