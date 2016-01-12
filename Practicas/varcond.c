#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

pthread_t * threads;
pthread_mutex_t * mutex;
pthread_cond_t * varcond;

int buffer;

void * consumidor(void * input) {
	printf("Comenzando consumidor!\n");

	while(1) {	
		pthread_mutex_lock(mutex);
			while(buffer == 0)
				pthread_cond_wait(varcond, mutex);
			printf("I eat an %d", buffer);
			buffer = 0;
			printf(" and left the buffer on %d\n", buffer);
			pthread_cond_signal(varcond);
		pthread_mutex_unlock(mutex);
	}

	return NULL;
}

void * productor(void * input) {
	printf("Comenzando productor!\n");

	while(1) {	
		pthread_mutex_lock(mutex);
			while(buffer != 0)
				pthread_cond_wait(varcond, mutex);
			buffer = rand() % 10 + 1;
			sleep(1);
			pthread_cond_signal(varcond);
		pthread_mutex_unlock(mutex);
	}

	return NULL;
}


int main(int argc, char * argv[]) {
	srand(time(NULL));

	// Mutex
	mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex, NULL);

	// Conditionals
	varcond = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
	pthread_cond_init(varcond, NULL);

		// Threads
		threads = (pthread_t *) malloc(sizeof(pthread_t) * 2);
		pthread_create(&threads[0], NULL, productor, NULL);
		pthread_create(&threads[1], NULL, consumidor, NULL);

	int i;
	for(i = 0; i < 2; i++)
		pthread_join(threads[i], NULL);

	printf("Programa terminado!\n");

	return EXIT_SUCCESS;
}