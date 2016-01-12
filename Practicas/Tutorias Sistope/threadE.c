#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *hello(void *arg){
	char *msg = "Hola mundo";
	printf("%s\n", msg);
}

int main(int argc, char *argv[]){
  pthread_t h1;
  pthread_create(&h1,NULL,hello,NULL);
  pthread_join(h1,NULL);
  return 0;
}