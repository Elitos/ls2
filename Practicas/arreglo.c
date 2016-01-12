#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define SIZE 40

int * a;
pthread_mutex_t * mutexs;
pthread_t * threads;
pthread_barrier_t * barrera;

void * hebra(void * input) {
	int tid = input;
	int myVal;
	int ptid = (tid == 0) ? SIZE-1 : tid-1;

	int minTid = (tid == 0) ? tid : ptid
	,maxTid = (tid == 0) ? ptid : tid;

	myVal = a[tid];

	pthread_barrier_wait(barrera);
	while(1) {
		pthread_mutex_lock(&mutexs[minTid]);
		pthread_mutex_lock(&mutexs[maxTid]);
			if(a[ptid] > myVal) {
				a[tid] = a[ptid];
			} else if(a[ptid] == myVal) {
				printf("El mayor es %d\n", myVal);
				exit(0);
			}
		pthread_mutex_unlock(&mutexs[maxTid]);
		pthread_mutex_unlock(&mutexs[minTid]);
	}
	printf("Hebra %d launched a[%d] = %d\n", tid, tid, a[tid]);
}

int main(int argc, char * argv[]) {
	srand(time(NULL));
	int i;

	// Mutex
	a = (int *) malloc(sizeof(int) * SIZE);
	threads = (pthread_t *) malloc(sizeof(pthread_t) * SIZE);
	mutexs = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * SIZE);
	barrera = (pthread_barrier_t *) malloc(sizeof(pthread_barrier_t));
	
	pthread_barrier_init(barrera, NULL, SIZE);
	
	for(i = 0; i < SIZE; i++) {
		a[i] = i+1;
		pthread_mutex_init(&mutexs[i], NULL);
	}

	for(i = 0; i < SIZE; i++) {
		pthread_create(&threads[i], NULL, hebra, i);
	}
	

	for(i = 0; i < SIZE; i++)
		pthread_join(threads[i], NULL);
	
	printf("Programa terminado!\n");

	return EXIT_SUCCESS;
}