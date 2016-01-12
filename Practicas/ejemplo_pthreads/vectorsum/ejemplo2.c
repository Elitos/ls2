#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

//Compilación gcc archivo.c -o archivoejecutable -lpthread
pthread_barrier_t   barrier;

typedef struct{
    int *A; //Vector A
    int *B; //Vector B
    int *result; //Vector resultado
}VectorsInfo;

VectorsInfo info;

void *vectorsum(void *arg){
	int tid = (intptr_t)arg;
	printf("Sumando tid = %d\n", tid);
	info.result[tid] = info.A[tid] + info.B[tid];
	//pthread_barrier_wait(&barrier);
}

int main(int argc, char *argv[]) {
	int n, i;
	pthread_t *threads;
	n = atoi(argv[1]);
	pthread_barrier_init (&barrier, NULL, n);
	pthread_attr_t pthread_attr;
	
	info.A = (int *)malloc(n*sizeof(int));
	info.B = (int *)malloc(n*sizeof(int));
	info.result = (int *)malloc(n*sizeof(int));
	
	for(i=0; i<n; i++){
		info.A[i] = i+1;
		info.B[i] = n-i;
	}
	
	threads = (pthread_t *)malloc(n*sizeof(pthread_t));
	pthread_attr_init(&pthread_attr);
	
	for(i=0; i<n; i++){
		printf("Creando thread: %d\n", i);
		pthread_create(&threads[i], &pthread_attr, vectorsum, (void *)(intptr_t)i);
	}
	
	
	
	for (i=0; i<n; i++){
		pthread_join(threads[i],NULL);
	}
	
	
	for (i=0; i<n; i++){
		printf("A[%d]= %d + B[%d]= %d, C[%d] = %d\n",i, info.A[i], i, info.B[i], i, info.result[i]);
	}
	
	
}
