#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LEER 0
#define ESCRIBIR 1


int main(int argc, char *argv[]){

	int pipe1[2];
	int pipe2[2];
	pipe(pipe1);
	pipe(pipe2);
    char str[100]; 

    int status = 0; 
    pid_t wpid;
     
	printf("Escritura: %d, Lectura: %d\n",pipe1[LEER],pipe1[ESCRIBIR]);
	printf("Escritura: %d, Lectura: %d\n",pipe2[LEER],pipe2[ESCRIBIR]);
    
    int pid = fork();

    if(pid == 0){// Proceso hijo
       dup2(pipe1[ESCRIBIR],STDOUT_FILENO);
	   close(pipe1[LEER]);
       close(pipe1[ESCRIBIR]);
       write(STDOUT_FILENO, "Anda a acostarte, es muy tarde!", 100);
       
       dup2(pipe2[LEER],STDIN_FILENO);
       close(pipe2[LEER]);
       close(pipe2[ESCRIBIR]);

    }else{ // Proceso Padre

       read(pipe1[LEER], str, 100); 
       printf("Padre pipe1: %s\n",str);
  
    }
    
    while ((wpid = wait(&status)) > 0)
      
   return 0;
}

/* dup2(a,b) a y b son descriptores de archivos como lo es pipe1[LEER] y los otros.
   Hace que b se conecte al descriptor que se conecta a

*/