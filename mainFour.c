#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// lock:
// critical part
// unlock


int locksObtained = 0;
int var1 = 3;
int lockVar = 0;
int oneValue = 1;
char* word = "asdf";

void criticalSection();

void unlock();



void *lock(){
    long threadID = (long) pthread_self();
    fprintf(stderr, "\nThread #%ld is trying to get the lock", threadID);
    // keep trying to get the lock,
    // if we fail, just restart the subroutine
    // else, print "we got the lock"
    // move on to critical section
    asm volatile ("LDR R7, =lockVar\n\t"
               "LDREX R1, [R7]\n\t"
               "CMP R1, #0\n\t"
               "MOV R9, #1\n\t" // set R9 equal to 1
               "STREX R2, R9, [R7]\n\t" // attempt to update the shared loc$
               "CMP R2, #0\n\t" // the store was successful if R2 = 0
               "BNE lock\n\t" // if R2 != 0, then restart
        );

    // will not get here unless the thread gets the lock
    criticalSection();
}

void criticalSection(){
    locksObtained += 1;    
    long winningID = (long) pthread_self();
    // here is critical section
    // ok here try moving the variable from R5 into C variable and printing it
    fprintf(stderr, "\nThread #%ld has won and is in the critical section.\n", $
    fprintf(stderr, "\n%d different locks have been obtained so far.\n", locksO$
    unlock();
}

void unlock(){
    long winningID = (long) pthread_self();
    fprintf(stderr, "\nunlocking Thread #%d\n", winningID);
    lockVar = 0;
}


int main(){
    int i;
    pthread_t tid;
    for (i = 0; i < 100; i++){
         pthread_create(&tid, NULL, lock, NULL);
    }

    pthread_exit(NULL);

    return 0;
}
