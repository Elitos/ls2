#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdio_ext.h>

#define LEER 0
#define ESCRIBIR 1
#define TRUE 1
#define FALSE 0


int main(int argc, char *argv[]){

  //char *str; 
  int pipe1[2];
  char str[100];
  int status = 0; 
  pid_t wpid; 

    int i;
    //for(i=0; i<5; i++){
     
      pipe(pipe1);
      
      printf("Escritura: %d, Lectura: %d\n",pipe1[LEER],pipe1[ESCRIBIR]);
      
      pid_t pid = fork();

        if(pid == 0){// Proceso hijo
    	     close(1);
           dup(pipe1[ESCRIBIR]);
           //close(0);
           //close(pipe1[LEER]);
           execl(argv[0],argv[1],(char*)NULL);
           perror("connect");  
        }else{ // Proceso Padre
           close(0);
           dup(pipe1[LEER]);
           //close(1);
           //close(pipe1[ESCRIBIR]);
           //str = (char*)malloc(sizeof(char)*50);
           read(pipe1[LEER], str, 100);
           //close(pipe1[LEER]);
           printf("Padre pipe1: %s\n", str);
           //free(str);
           //flush(STDOUT);
           //dup2(pipe1[ESCRIBIR],STDOUT_FILENO);  
           
        }
    //}

      
   return 0;
}
