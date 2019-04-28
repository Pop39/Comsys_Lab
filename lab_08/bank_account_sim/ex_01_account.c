#include "ex_01_account.h"

static struct timespec tim, tim2;

void account_init(account_info *sp, int n)
{
    int i;
        
    tim.tv_sec = 0;
    tim.tv_nsec = 100;
    for (i=0; i<n; i++) {
        sprintf(sp[i].name, "%s%d", "test_account", i);
        sp[i].id = i;
        sp[i].balance = INT_BALANCE;

        sp[i].ptr_borrow_sem = (sem_t*)malloc( sizeof( sem_t ) );
        sp[i].ptr_lender_sem = (sem_t*)malloc( sizeof( sem_t ) );
        sp[i].ptr_data_sem = (sem_t*)malloc( sizeof( sem_t ) );
        sem_init( sp[i].ptr_borrow_sem  , 0 , 0 );
        sem_init( sp[i].ptr_lender_sem  , 0 , 0 );
        sem_init( sp[i].ptr_data_sem  , 0 , 1 );
    }
}

void account_deposit(account_info *sp, int tid, int amount)
{
    int temp1, temp2;

    temp1 = sp[tid].balance;
    temp2 = amount + temp1;
    nanosleep(&tim, &tim2);
    sp[tid].balance = temp2;
}

void account_withdraw(account_info *sp, int tid, int amount)
{
    int temp1, temp2;

    temp1 = sp[tid].balance;
    while (temp1 < amount)
    {
        //printf("Not enough funds to withdraw!\n");
        return;
    }
    temp2 = temp1 - amount;
    nanosleep(&tim, &tim2);
    sp[tid].balance = temp2;
}

