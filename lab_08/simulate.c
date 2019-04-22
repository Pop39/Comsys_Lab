#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "account.h"

account_info *account_database;

void *lender(void *arg)
{
    int tid = (int)arg;
    int account_id = tid/2;
    int i;

    if( tid%2 == 0 ) // this will make you know that first sequence in order or not?
    {
        sem_post( &(account_database[account_id].go_lender ) );
    }

    sem_wait( &(account_database[account_id].go_lender ) );
    sem_wait( &(account_database[account_id].go_edit ) );

    printf("I am lender # %d on thread id is %d\n", account_id , tid );
    for (i=0; i<ROUND; i++) {
        account_deposit(account_database, account_id , 2);
    }

    sem_post( &(account_database[account_id].go_edit ) );
    sem_post( &(account_database[account_id].go_borrow ) );
  
    return NULL;
} // function lender

void *borrower(void *arg)
{
    int tid = (int)arg;
    int account_id = tid/2;
    int i;

    if( tid%2 == 0 ) // this will make you know that first sequence in order or not?
    {
        sem_post( &(account_database[tid/2].go_borrow) );
    }
    
    sem_wait( &(account_database[account_id].go_borrow ) );
    sem_wait( &(account_database[account_id].go_edit ) );

    printf("I am borrower # %d on thread id is %d\n", account_id , tid );
    for (i=0; i<ROUND; i++) {
        account_withdraw(account_database, account_id , 1);
    }

    sem_post( &(account_database[account_id].go_edit ) );
    sem_post( &(account_database[account_id].go_lender ) );
  
    return NULL;
} // function borrower

int main(int argc, char *argv[])
{

    int i, num_account;

    num_account = 4;
    printf("size of account_info is %ld; size of int is %ld; size of void pointer is %ld\n"
            , sizeof(account_info), sizeof(int), sizeof(void *));
    account_database = (account_info *)malloc(sizeof(account_info)*num_account);
    pthread_t tid[num_account*2];
    if (account_database == NULL) {
        fprintf(stderr, "Malloc unsuccessful\n");
        exit(0);
    }
    // Initialize account_database
    account_init(account_database, num_account);
    for (i=0; i<num_account; i++) {
        printf("Account: %d; name: %s; balance: %d\n", i, account_database[i].name, account_database[i].balance);
    }
    printf("\n");
    for (i=0; i < num_account*2 ; ) {
        pthread_create( &(tid[i]) , NULL , lender , (void *)i );
        i++;
        pthread_create( &(tid[i]) , NULL , borrower , (void *)i );
        i++;
    }
    printf("\n");
    for( i = 0 ; i < num_account*2 ; i++ ){
        pthread_join( tid[i] , NULL );
    }
    for (i=0; i<num_account; i++) {
        printf("Account: %d; name: %s; balance: %d\n", i, account_database[i].name, account_database[i].balance);
    } 
} // function main
