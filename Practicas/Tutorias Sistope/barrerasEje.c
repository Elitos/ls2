#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef struct{
	int *A;
	int *B;
	int *result;
}VectorsInfo;

VectorsInfo info;

void *vector_sum(void *arg){
	int tid = (intptr_t)(arg);
    printf("Sumando tid = %d\n",tid);
    info.result[tid] = info.A[]
}


int main(int argc, char *argv[]){
  pthread_t h1;
  pthread_create(&h1,NULL,hello,NULL);
  pthread_join(h1,NULL);
  return 0;
}