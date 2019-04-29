#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ex_01_account.h"

account_info *account_database;

void *lender(void *arg)
{
    int tid = (int)arg;
    int account_id = tid/2;
    int i;

    if( (tid%2) == 0 )
    {
        sem_post( account_database[ account_id ].ptr_lender_sem );
    }

    sem_wait( account_database[ account_id ].ptr_lender_sem );
    printf("I am lender # %d of account id %d\n", tid , account_id );
    for (i=0; i<ROUND; i++) {
        sem_wait( account_database[ account_id ].ptr_data_sem );
        account_deposit(account_database, account_id, 2);
        sem_post( account_database[ account_id ].ptr_data_sem );
    }
    sem_post( account_database[ account_id ].ptr_borrow_sem );
  
    return NULL;
} // function lender

void *borrower(void *arg)
{
    int tid = (int)arg;
    int account_id = tid/2;
    int i;

    if( (tid%2) == 0 )
    {
        sem_post( account_database[ account_id ].ptr_borrow_sem );
    }

    sem_wait( account_database[ account_id ].ptr_borrow_sem );
    printf("I am borrower # %d of account id %d\n", tid , account_id );
    for (i=0; i<ROUND; i++) {
        sem_wait( account_database[ account_id ].ptr_data_sem );
        account_withdraw(account_database, account_id, 1);
        sem_post( account_database[ account_id ].ptr_data_sem );
    }
    sem_post( account_database[ account_id ].ptr_lender_sem );
  
    return NULL;
} // function borrower

int main(int argc, char *argv[])
{

    int i, num_account , num_thread;

    num_account = 4;
    num_thread = num_account * 2; 
    
    pthread_t all_tid[ num_thread ];

    printf("size of account_info is %d; size of int is %d; size of void pointer is %d\n"
            , sizeof(account_info), sizeof(int), sizeof(void *) );
    account_database = (account_info *)malloc(sizeof(account_info)*num_account);
    if (account_database == NULL) {
        fprintf(stderr, "Malloc unsuccessful\n");
        exit(0);
    }
    // Initialize account_database
    account_init(account_database, num_account);
    for (i=0; i<num_account; i++) {
        printf("Account: %d; name: %s; balance: %d\n", i, account_database[i].name
                , account_database[i].balance);
    }
    printf("\n");
    printf( "Start thread account %d threads\n" , num_thread );
    for (i = 0 ; i < num_thread ; ) {
        pthread_create( &( all_tid[ i ] ) , NULL , lender , (void *)(i) );
        i++;
        pthread_create( &( all_tid[ i ] ) , NULL , borrower , (void *)(i) );
        i++;
    }
    printf("\n");

    for (i=0; i<num_account; i++) {
        pthread_join( (all_tid[ (i * 2) ] ) , NULL );
        pthread_join( (all_tid[ (i * 2) + 1] ) , NULL );
        printf("Account: %d; name: %s; balance: %d\n", i, account_database[i].name
                , account_database[i].balance);
    } 
    return 0;
}
