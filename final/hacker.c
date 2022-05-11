#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int lockVar = 0;
int accountBalance = 100;
pthread_barrier_t barrier;

void *loadLinkRoutine(){
    long threadID = (long) pthread_self();
    int realBal = accountBalance;
     __asm__ volatile(
    "lock:\n\t"
    "try_again:\n\t"
   // "MOV R3, %[lock]\n\t" // move lock variable into R3
    "LDR R10, =lockVar\n\t"
    "MOV R7, #0\n\t"
    "LDREX R8, [R10]\n\t" // load value stored at that address
    "NOP \n\t"
    "CMP R8, R7\n\t"
    "NOP \n\t"
    "BNE try_again\n\t"
    "MOV R9, #1\n\t"
    "STREX R2, R9, [R10]\n\t"
    "CMP R2, R7\n\t"
    "BNE try_again\n\t"
    // if we make it here, got the lock
    // critical section - modify shared variable here
    "MOV R4, #5\n\t"
    "add %[DEST], R4\n\t"

    // okay, shared variable has been modified
    // now, replace the lock
    "MOV R5, #0\n\t"
    "LDR R10, =lockVar\n\t"
    "STR R5, [R10]\n\t"
       
    : [DEST] "=r" (accountBalance)
    //  [balRes] "=r" (accountBalance)
    : "[DEST]" (accountBalance)
);
    pthread_barrier_wait(&barrier);
}

int main(){
    int i;
    pthread_t t[3];
    pthread_barrier_init(&barrier, NULL, 4);

    pthread_create(&t[0], NULL, loadLinkRoutine, NULL);
    pthread_create(&t[1], NULL, loadLinkRoutine, NULL);
    pthread_create(&t[2], NULL, loadLinkRoutine, NULL);

    pthread_barrier_wait(&barrier);
    fprintf(stderr, "after all threads have run, the value of account balance is %d\n", acco$
    pthread_barrier_destroy(&barrier);
    return 0;
}
