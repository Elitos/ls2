#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

//Compilación gcc archivo.c -o archivoejecutable -lpthread

void *hello(void *arg){
	char *msg = "Hola";
	printf ("%s\n" , msg);

}

void *world(void *arg) {
	char *msg = "mundo";
	printf ("%s\n", msg) ;

}

int main(int argc, char *argv[]) {
	pthread_t h1;
	pthread_t h2;
	pthread_create(&h1, NULL, hello, NULL);
	pthread_create(&h2, NULL, world, NULL);
	pthread_join (h1, NULL);
	pthread_join (h2, NULL);
	return 0;
}
