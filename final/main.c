#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int lockVar = 0;

void *loadLinkRoutine(){
    long threadID = (long) pthread_self();

    __asm__ volatile(
    "lock:\n\t"
    "try_again:\n\t"
   // "MOV R3, %[lock]\n\t" // move lock variable into R3
    "LDR R10, =lockVar\n\t"
    "LDREX R8, [R10]\n\t" // load value stored at that address
    "CMP R8, #0\n\t"
    "BNE try_again\n\t"
    "MOV R9, #1\n\t"
    "STREX R2, R9, [R10]\n\t"
    "CMP R2, #0\n\t"
    "BNE try_again\n\t"
    // if we make it here, got the lock

    "MOV R5, #0\n\t"
    "LDR R10, =lockVar\n\t"
    "STR R5, [R10]\n\t"
    //"LDREX R3, =lockVar" // [lock must be an address]
    ::
      [lock] "r" (lockVar)
);
}

int main(){
    int i;
    pthread_t tid;
    for (i = 0; i < 10; i++){
         pthread_create(&tid, NULL, loadLinkRoutine, NULL);
    }

    pthread_exit(NULL);

    return 0;
}
