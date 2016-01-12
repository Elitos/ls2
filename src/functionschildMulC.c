#include "functions.h"

int main (int argc, char *argv[]) { 
 
 Matrix A, B; 
 // argv[1] = tamA y argv[2] = TamB
 int tamBuffer = atoi(argv[1])*2;

 char buff[tamBuffer];

 int nread = read(STDIN_FILENO,buff,tamBuffer);
 
 char *buffer = (char *)malloc(sizeof(char)*tamBuffer);
 strcpy(buffer,buff);
 int tamA = atoi(argv[1]);

 int b = atoi(argv[2]);


 //char  *pA = strtok(buffer,"");

 SToMatriz(buffer,&A);

 AmulbcToB(A,b,&B);
    
 char sizeA[10];
 sprintf(sizeA, "%d", b);
 
if(B.n != 0){
  write(STDOUT_FILENO,MatrizToS(B),tamBuffer*numPlaces(b));
}else{ 
  write(STDOUT_FILENO,"!",2);
}
 free(buffer);

 exit(0);
 return 0;
}