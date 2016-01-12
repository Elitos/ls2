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

int getComando(char *comando, char *array[]){
  int i;
  int j;
  char *p;
  //char *array[5];
  i = 0;
  p = strtok (comando," ");  
  while (p != NULL)
  {
    array[i++] = p;
    p = strtok (NULL, " ");
  }
  int noNull = i;
  if(i != 4){
     for(j = i; i<5; i++){
         array[i] = "\0";
     }
  }
  //for (i=0;i<5; ++i) 
    //printf("%s\n", array[i]);
 
  return noNull;
}

void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

int isComandoP_O_H(char *array[], int nargs){ // Padre 1 , Hijo 2 , error -1
    
    if(nargs == 3){
      if(strcmp(array[0],"save") == 0 || strcmp(array[0],"load") == 0){ // load o save
       if(array[1][0] >= 65 && array[1][0] <= 90){ // load o save A
          removeChar(array[2],'"');
          printf(" array sin \" %s\n",array[2]); // load o save A "A.txt"
          //if(array[2][0] >= 65 && array[2][0] <=90 && array[2][1] == '.' && (strstr(array[2], ".txt")) != NULL && array[2][2] == 't' && array[2][3] == 'x' && array[2][4] == 't'){
          if(array[2][0] >= 65 && array[2][0] <=90 && (strstr(array[2], ".txt")) != NULL){
             return 1;
          }
       }
      }
      if(array[0][0] >= 65 && array[0][0] <= 90){ // A
         if(array[1][0] == '='){ //A =
            if(array[2][0] >= 65 && array[2][0] <= 90){ // A = B
             return 1;
            }
         }
      }  
    }

    if(nargs == 2){
      if(strcmp(array[0],"print") == 0 || strcmp(array[0],"clear") == 0){ // print o clear
       if(array[1][0] >= 65 && array[1][0] <= 90){ // print o clear A
        return 1;
       }
      }  
    }
    
    if(nargs == 5){
        if(strlen(array[0])==1 && array[0][0] >= 65 && array[0][0] <= 90){ // tengo el A
            if(strlen(array[1])==1 && array[1][0] == '='){
                if(strlen(array[2])==1 && array[2][0] >= 65 && array[0][0] <= 90){ // reconosco A = A
                   if((strlen(array[3]) == 1) && (array[3][0] == '+' || array[3][0] == '-' || array[3][0] == '*')){ //reconosco A = A *
                      if(strlen(array[4]) == 1 && array[4][0] >= 65 && array[4][0] <= 90){ // reconosco A = A * A
                        return 2;
                      }
                   }
                }
            }
        }
    }

    if(nargs == 4){
        if(strlen(array[0])==1 && array[0][0] >= 65 && array[0][0] <= 90){ // tengo A
           if(strlen(array[1])==1 && array[1][0] == '='){ // tengo A = 
              if(strcmp(array[2],"trans") == 0){  // tengo A = trans
                 if(strlen(array[3])==1 && array[3][0] >= 65 && array[3][0] <= 90){ // tengo A = trans A
                    return 2;
                 }
              }
           }
        }
    }
    return -1;
}

int getFuncion(char *array[], int nargs){
    if (isComandoP_O_H(array,nargs) == 1){
       if(nargs == 3){
          if(strcmp(array[0],"save") == 0){ // load o saveA
            return 1;
          }
          if(strcmp(array[0],"load") == 0){
            return 2;
          } 
          if(array[1][0] == '='){
            return 3;
          }
        }

       if(nargs == 2){
          if(strcmp(array[0],"print") == 0){ 
            return 4;
          }
          if(strcmp(array[0],"clear") == 0){ // print o clear A
            return 5;
          } 
       }
   }
   if(isComandoP_O_H(array,nargs) == 2){
      if(nargs == 5){
         if(array[3][0] == '+'){
            return 6;
         }
         if(array[3][0] == '-'){
            return 7;
         }
         if(array[3][0] == '*'){
            if(atoi(array[4]) != 0){
                return 8;
            }else{
                return 9;
            }
         }
      }
     if(nargs == 4){ 
        if(strcmp(array[2],"trans") == 0){  // tengo A = trans
           return 10;
        }
     }  
   }
     return -1;
}

int main(int argc, char * argv[]){

    char comando[15] = "Co!";

    while(strcmp(comando,"exit") != 0){
		while(getLine("Ingrese comando: \n",comando,sizeof(comando)) == NO_INPUT){            
		}
        
        char *s[5]; 
		    int nargs = getComando(comando, s);
        printf("%d\n", nargs);
        int valido = isComandoP_O_H(s,nargs);
        printf("%s , valido : %d, funcion : %d\n", s[2], valido, getFuncion(s,nargs));
    }
	
}
