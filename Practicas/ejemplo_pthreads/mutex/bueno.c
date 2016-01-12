#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t tid[2];
int contador;
pthread_mutex_t lock;

void* function(void *arg)
{
    pthread_mutex_lock(&lock);

    unsigned long i = 0;
    contador += 1;
    printf("\n Trabajo %d comenzado\n", contador);

    for(i=0; i<(0xFFFFFFFF);i++);

    printf("\n Trabajo %d terminado\n", contador);

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(void)
{
    int i = 0;
    int err;

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n No se puede iniciar el mutex\n");
        return 1;
    }

    while(i < 2)
    {
        err = pthread_create(&(tid[i]), NULL, function, NULL);
        if (err != 0)
            printf("\nNo se pudo crear el thread :[%s]", strerror(err));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}
