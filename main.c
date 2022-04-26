#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// lock:
// critical part
// unlock


int var1 = 3;
int lockVar = 0;
int *pLockVar;
int oneValue = 1;
char* word = "asdf";

void criticalSection();
void unlock();

// okay this makes sure that R7 holds the lock
void setup(){
        pLockVar = &lockVar;
}

void *lock(void *threadArg){
        int *myid = (int *)threadArg;
        fprintf(stderr, "\nthread %d is attempting to get lock", *myid);
        // keep trying to get the lock,
        // if we fail, just restart the subroutine
        // else, print "we got the lock"
        // move on to critical section
        __asm__("LDR R7, =lockVar\n\t"
               "LDREX R1, [R7]\n\t"
               "CMP R1, #0\n\t"
               "BNE lock\n\t"
               "MOV R9, #1\n\t" // set R9 equal to 1
               "STREX R2, R9, [R7]\n\t" // attempt to update the shared loc$
               "CMP R2, #0\n\t" // the store was successful if R2 = 0
               "BNE lock\n\t" // if R2 != 0, then restart
               "BL criticalSection\n\t"); // else, move on to critical section
}

void criticalSection(){
        // here is critical section
        fprintf(stderr, "\nthread has gotten the lock and is in critical section");
        __asm__("BL unlock");
}

void unlock(){
        // okay, we're done with the critical section, give up lock
        // don't need STREX here because we know that
        // here we own the lock 
        fprintf(stderr, "\nunlocking");
        __asm__("LDR R7, =lockVar\n\t"
                "MOV R10, #0\n\t"
                "STR R10, [R7]");
        fprintf(stderr, "\nfinished unlocking");
}
static pthread_barrier_t bar;

static void* thrfunc(void *arg){
        pthread_barrier_wait(&bar);
}
int main(){
        fprintf(stderr, "%s", "main working\n");
        pthread_barrier_init(&bar, NULL, 3);
        pthread_t tid;
        int i;
        setup();
        for (i = 0; i < 4; i++)
        {
                pthread_create(&tid, NULL, lock, (void *)&tid);
        //      fork();
        }
        fprintf(stderr, "\nexiting thread");
        pthread_barrier_destroy(&bar);
        pthread_exit(NULL);
        fprintf(stderr, "\n ok returning");
        return 0;
}
