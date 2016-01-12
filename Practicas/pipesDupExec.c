/*
LABORATORIO 1 SISTEMAS OPERATIVOS 2-2015
Integrantes
Elías Gonzalez 18.248.829-1
Prof: Fernando Rannou - Ayudante: Luis Loyola
*/

/*
Header donde se declaran las funciones a utilizar por el proceso principal (padre), con sus respectivos
includes a las librerias necesarias.
*/

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/select.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
#define LEER 0
#define ESCRIBIR 1

static int getLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

int main(int argc, char *argv[]){

	int readpipe[2];
  int writepipe[2];
  char * c ;
  char str[100];
    
  pid_t pid;
  time_t t;
  int status;

char comando[10] = "Co!";
int comando_lengh = 0; 
while(strcmp(comando,"exit") != 0){
      
      pipe(readpipe);
      pipe(writepipe);

      if ((pid = fork()) < 0)
        perror("fork() error");
      else if (pid == 0) {
        close(writepipe[1]);
        close(readpipe[0]);

        dup2(writepipe[0],  0);  close(writepipe[0]);
        dup2(readpipe[1], 1);  close(readpipe[1]);
        execv("./otro", (char *[]){ "./otro", comando, NULL });;
        perror("execv() error");
        _exit(1);
      }
      else{ 
            close(writepipe[0]);
            close(readpipe[1]);
            char *c = (char*)malloc(sizeof(char)*100); 
            char *p = (char*)malloc(sizeof(char)*100); 
            strcpy(p,"mensaje de padre a hijo");
            write(writepipe[1]_DES, p , 100); 
            int nread = read(readpipe[0]_DES, c, tamB*tamA);
            free(p);
            printf("%s\n",c);
            close(readpipe[0]);
            close(writepipe[1]);
      }
 
   while(getLine("",comando,sizeof(comando)) == NO_INPUT){}
    // gets(comando);
  
} 

   return 0;
}
