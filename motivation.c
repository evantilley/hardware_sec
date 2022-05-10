#include <stdio.h>
#include <pthread.h>

int accountBalance = 0;

void *increaseBalance(){
    int i;
    for (i = 0; i < 3000; i++){
        accountBalance += 1;
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
