#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define INT_BALANCE 40000
#define ROUND 40000

typedef struct {
    char name[31];
    int id;
    int balance;
    sem_t *ptr_write_sem;
    sem_t *ptr_read_sem; 
} account_info;

void account_init(account_info *sp, int n);
void account_deposit(account_info *sp, int account_id, int amount);
void account_withdraw(account_info *sp, int account_id, int amount);
void account_writing( account_info *sp , int account_id , int result );
