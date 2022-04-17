#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int var1 = 3;


void  *lockit(void *vargp){
	int *myid =(int *) vargp;
	fprintf(stderr, "lockit called by %d\n", *myid);
	__asm__("LDR R0, =var1\n\t"
	"LDREX R1, [R0]\n\t"
	"MOV R5, #4\n\t"
	"ADD R1, R1, R5\n\t"
	"STREX R2, R1, [R0]\n\t"
	"SVC 0");
}

int main(){
        fprintf(stderr, "%s", "main working\n");
	pthread_t tid;
	int i;
	for (i = 0; i < 2; i++)
	{
		pthread_create(&tid, NULL, lockit, (void *)&tid);
	}
	pthread_exit(NULL);
        return 0;
}

