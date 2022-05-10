#include <stdio.h>
#include <pthread.h>

int lockVar = 0;
int accountBalance = 0;

void *increaseBalance(){
    int i;
    for (i = 0; i < 3000; i++){
        __asm__ volatile (
        "lock:\n\t"
        "retry:\n\t"
        "LDR R10, =lockVar\n\t"
        "LDREX R8, [R10]\n\t"
        "CMP R8, #0\n\t"
        "BNE retry\n\t"
        "MOV R9, #1\n\t"
        "STREX R2, R9, [R10]\n\t"
        "CMP R2, #0\n\t"
        "BNE retry\n\t"

        // critical section
        "MOV R4, #1\n\t"
        "add %[DEST], R4\n\t"

        "MOV R5, #0\n\t"
        "LDR R10, =lockVar\n\t"
        "STR R5, [R10]\n\t"
        :    [DEST] "=r" (accountBalance)
    : "[DEST]" (accountBalance)
        );
            }

}

int main(){
    int listLength = 3;
    pthread_t thread[listLength];

    for (int i = 0; i != listLength; i++) {
        if (pthread_create(&thread[i], NULL, &increaseBalance, NULL) != 0) {
            printf("ERROR : pthread create failed.\n");
            return (0);
        }
    }
    for (int i = 0; i != listLength; i++) {
        if (pthread_join(thread[i], NULL) != 0) {
            printf("ERROR : pthread join failed.\n");
            return (0);
        }
    }
    fprintf(stderr, "balance is %d\n", accountBalance);
    return 0;
}
