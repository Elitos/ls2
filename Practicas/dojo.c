#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

pthread_barrier_t our_barrier;

void * funcion (void * input)
{
	int i = (int) input;
	printf ("Hebra numero %d\n", i);
        //pthread_barrier_wait(&our_barrier);
}

void main ( int argc, char* argv[])
{
	pthread_barrier_init(&our_barrier, NULL, atoi(argv[1]));
	pthread_t *hebra = (pthread_t*) malloc(sizeof(pthread_t)*atoi(argv[1]));

	int i;
	
	for (i=0; i<atoi(argv[1]); i++){
		pthread_create(&hebra[i],NULL,&funcion,(void*)i);
		
	//for (i=0; i<atoi(argv[1]); i++){
		pthread_join(hebra[i],NULL);
	}

	printf("Terminado\n");

}
