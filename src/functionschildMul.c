#include "functions.h"

int main (int argc, char *argv[]) { 
 
 Matrix A, B, C;
 C.n = 0; 
 // argv[1] = tamA y argv[2] = TamB
 int tamBuffer = (atoi(argv[1])+atoi(argv[2])*2);

 char buff[tamBuffer];

 int nread = read(STDIN_FILENO,buff,tamBuffer);
 
 char *buffer = buff;
 int tamA = atoi(argv[1]);
 int tamB = atoi(argv[2]);

 char  *pA = strtok(buffer,",");
 char  *pB = strtok(NULL,",");

 SToMatriz(pA,&A);
 SToMatriz(pB,&B);

 AmulBToC(A,B,&C);
 
if(C.n != 0){
  write(STDOUT_FILENO,MatrizToS(C),tam_matriz_string(C));
}else{ 
  write(STDOUT_FILENO,"!",2);
}
 exit(0);
 return 0;
}