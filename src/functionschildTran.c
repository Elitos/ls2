#include "functions.h"

int main (int argc, char *argv[]) { 
 
 Matrix A, B;
 B.n = 0; 
 // argv[1] = tamA y argv[2] = TamB
 int tamBuffer = atoi(argv[1])*2;

 char buff[tamBuffer];

 int nread = read(STDIN_FILENO,buff,tamBuffer);
 
 char *buffer = buff;
 int tamA = atoi(argv[1]);

 char  *pA = strtok(buffer,",");

 SToMatriz(pA,&A);

 ATransToB(A,&B);
    
if(B.n != 0){
  write(STDOUT_FILENO,MatrizToS(B),tamBuffer);
}else{ 
  write(STDOUT_FILENO,"!",2);
}
 exit(0);
 return 0;
}