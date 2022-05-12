#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int lockVar = 0;
char *lockMessage;
int lockMessageLength;

char* threadMessage;
int threadMessageLength;

char threadIDString[256];
int threadIDLength;

void *loadLinkRoutine(){
    long threadID = (long) pthread_self();
    threadMessage = "Thread ";
    threadMessageLength = strlen(threadMessage);

    char str[11];
    sprintf(str, "%ld\n", threadID);
    //threadIDString = str;
    threadIDLength = strlen(str);

    lockMessage = " is Attempting to get lock\n";
    lockMessageLength = strlen(lockMessage);
    __asm__(
    "lock:\n\t"
    "MOV R0, #2\n\t" // output to stderr
    "MOV R1, %[messageOne]\n\t" // load in message
    "MOV R2, %[messageOneLength]\n\t" // load in messageLength
    "MOV R7, #4\n\t" // tell system we want to print
    "SWI 0\n\t" // syscall

    "MOV R0, #2\n\t" // output to stderr
    "MOV R1, %[messageTwo]\n\t" // load in message
    "MOV R2, %[messageTwoLength]\n\t" // load in messageLength
    "MOV R7, #4\n\t" // tell system we want to print
    "SWI 0\n\t" // syscall    

    "try_again:\n\t"
   // "MOV R3, %[lock]\n\t" // move lock variable into R3
    "LDR R3, =lockVar\n\t"
    "LDREX R8, [R3]\n\t" // load value stored at that address
    "CMP R8, #0\n\t"
    "BNE try_again\n\t"
    "MOV R9, #1\n\t"
    "STREX R2, R9, [R3]\n\t"
    "CMP R2, #0\n\t"
    "BNE try_again\n\t"
    "MOV R5, #0\n\t"
    "LDR R3, =lockVar\n\t"
    "STR R5, [R3]\n\t"    //"LDREX R3, =lockVar" // [lock must be an address]
    :
    : [messageOne] "r" (threadMessage),
      [messageOneLength] "r" (threadMessageLength),
      [messageTwo] "r" (str),
      [messageTwoLength] "r" (threadIDLength),
      [messageThree] "r" (lockMessage),
      [messageThreeLength] "r" (lockMessageLength),
      [lock] "r" (lockVar)
);
}
int main(){
    lockMessage = "Attempting to get lock\n";
    lockMessageLength = strlen(lockMessage);
    int i;
    pthread_t tid;
    for (i = 0; i < 10; i++){
         pthread_create(&tid, NULL, loadLinkRoutine, NULL);
    }

    pthread_exit(NULL);

    return 0;
}
