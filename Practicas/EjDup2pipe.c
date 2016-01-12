#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LEER 0
#define ESCRIBIR 1

int main (int argc, char *argv[]) {

 int pipe1[2];
 pipe(pipe1);
 
 extern char **environ;

 char *const parmList[] = {"-l", "parametro"};

 if (fork()==0) { /* codigo del hijo */
   execl(argv[1],(const char *)environ, parmList);/* Porque se usa execlp ?*/
   perror("connect");
 } else { /* codigo del padre */
   //execl(argv[1],(const char *)parmList,(char*)NULL);/* Porque se usa execlp ?*/
   perror("connect");
 }

            int k; int n = 1;
        for(k=0;k<=n;k++){
            wait((int *)0);
        }

 return 0;

}