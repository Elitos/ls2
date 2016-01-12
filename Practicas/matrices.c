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
#define NUM_MATRIZ 26

typedef struct $Matris{
  char name;
  int n;
  int m;
  int **matriz;

}Matrix;

int **inicializarMatrizMem(int n, int m){
   int **matriz;
   matriz = (int **)malloc (n*sizeof(int *));
   int i;
   for (i=0;i<n;i++){
      matriz[i] = (int *) malloc (m*sizeof(int));
   }
   return matriz;
}

void inicializarTodasMatrizMem(Matrix *arr[]){
    int i;
    char abc[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    for(i = 0; i<NUM_MATRIZ; i++){
        arr[i]->n = 0;
        arr[i]->m = 0; 
        arr[i]->name = abc[i];
    }
    arr[i]->n = 0;
    arr[i]->m = 0;
    arr[i]->name = '\0';
}

void mostrarMatriz(Matrix M){
  if(M.n != 0){
   printf("Matriz %c [%d][%d]\n",M.name,M.n,M.m);
   int i;
   int j;
   for (i=0;i<M.n;i++){
     for(j=0;j<M.m;j++){
       printf("[%d]", M.matriz[i][j]);
     }
     printf("\n");
   }
  }else{
     printf("Matriz %c vacia\n",M.name);
  }
}

void removeChar(char *str, char garbage) {
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

int getComandos(char *comando, char *array[], int nargs){
  int i;
  int j;
  char *p;
  i = 0;
  p = strtok (comando," ");  
  while (p != NULL)
  {
    array[i++] = p;
    p = strtok (NULL, " ");
  }
  int noNull = i;
  if(i != nargs){
     for(j = i; i<nargs+1; i++){
         array[i] = "\0";
     }
  }
  for (i=0;i<nargs; ++i) 
    //printf("%s\n", array[i]);
 
  return noNull;
}

void fileToMatriz(Matrix * M, char *filename, char name_matriz){ 
 FILE *stream;
 char *line = NULL;
 size_t len = 0;
 ssize_t read;

 stream = fopen(filename, "r");
 if (stream == NULL)
     exit(EXIT_FAILURE);
 
 int i = 0;
 while ((read = getline(&line, &len, stream)) != -1) {
     removeChar(line,'\n');
     if(i == 0){
        M->n = atoi(line);
     }
     if(i == 1){
        M->m = atoi(line);
        M->matriz = inicializarMatrizMem(M->n,M->m);
        M->name = name_matriz; 
     }
     else if(i != 0 && i != 1){ // Leo primera linea i = 2
       int j;
       char *array[M->m];
       //printf("%d,%d\n",M->n,M->m);
       getComandos(line,array,M->m);
       for(j = 0;j<M->m;j++){
           M->matriz[i-2][j] = atoi(array[j]);
       }
       //printf("Retrieved line of length %zu :\n", read);
       //printf("%s", line);
     }
     i++;
 }

 free(line);
 fclose(stream);
}

void matrizToFile(Matrix *M, char *filename, char name_matriz){
 FILE *stream;
 size_t len = 0;
 ssize_t write;

 char buffer[20];

 stream = fopen(filename, "w");
 if (stream == NULL)
     exit(EXIT_FAILURE);
 
 sprintf(buffer, "%d", M->n);
 fprintf(stream, "%s\n",buffer);
 
 sprintf(buffer, "%d", M->m);
 fprintf(stream, "%s\n",buffer);
    
    
 int i = 0; int j; int k; 
 int total = M->n * M->m;
 //printf(" Total: %d\n", total);


 for (j = 0; j<M->n; j++){
     for(k = 0; k<M->m; k++){
         sprintf(buffer, "%d", M->matriz[j][k]);
         fprintf(stream, "%s ", buffer); 
     }
     fprintf(stream, "%c", '\n');
 }


 //free(line);
 fclose(stream);
}

void cleanMatriz(Matrix *M){
   if(M->n != 0){
   free(M->matriz);
   M->n = 0;
   M->m = 0;
   }
} 

void MaToMa(Matrix *destino, Matrix *inicio){ // A == A
  cleanMatriz(destino);
  destino->matriz = inicializarMatrizMem(inicio->n,inicio->m); // reservo espacio
  destino->n = inicio->n;
  destino->m = inicio->m;
  int i;
  int j;
  for(i = 0; i<inicio->n; i++){
      for(j = 0; j<inicio->m; j++){
          destino->matriz[i][j] = inicio->matriz[i][j];
      }
  }
}

int validarSumaResta(Matrix A, Matrix B){
  if(A.m == B.m && A.n == B.n && A.m != 0 && B.m != 0 && A.n != 0 && B.n != 0){
     return 1;
  }
  return 0;
}

void ASumBToC(Matrix A, Matrix B, Matrix *C){
   Matrix Aux;
   if(validarSumaResta(A,B)){
     Aux.matriz = inicializarMatrizMem(A.n,A.m);
     Aux.n = A.n;
     Aux.m = A.m;
     int i; int j;
     for(i = 0;i<A.n; i++){
         for (j = 0; j<A.m ; j++){
            Aux.matriz[i][j] = A.matriz[i][j] + B.matriz[i][j];
         }
     }
     //mostrarMatriz(Aux);
     cleanMatriz(C);
     C->matriz = inicializarMatrizMem(Aux.n,Aux.m);
     C->n = Aux.n;
     C->m = Aux.m;
     for(i = 0; i<Aux.n; i++){
         for(j = 0; j<Aux.m; j++){
            C->matriz[i][j] = Aux.matriz[i][j];
         }
     }
   }else{
     printf("Matrices no validas\n");
   }
}

void AResBToC(Matrix A, Matrix B, Matrix *C){
  Matrix Aux;
   if(validarSumaResta(A,B)){
     Aux.matriz = inicializarMatrizMem(A.n,A.m);
     Aux.n = A.n;
     Aux.m = A.m;
     int i; int j;
     for(i = 0;i<A.n; i++){
         for (j = 0; j<A.m ; j++){
            Aux.matriz[i][j] = A.matriz[i][j] - B.matriz[i][j];
         }
     }
     //mostrarMatriz(Aux);
     cleanMatriz(C);
     C->matriz = inicializarMatrizMem(Aux.n,Aux.m);
     C->n = Aux.n;
     C->m = Aux.m;
     for(i = 0; i<Aux.n; i++){
         for(j = 0; j<Aux.m; j++){
            C->matriz[i][j] = Aux.matriz[i][j];
         }
     }
   }else{
     printf("Matrices no validas\n");
   }
}

void AmulbToB(Matrix A, int b, Matrix *B){
   Matrix Aux;
   if(A.n != 0 && A.m != 0){
     Aux.matriz = inicializarMatrizMem(A.n,A.m);
     Aux.n = A.n;
     Aux.m = A.m;
     int i; int j;
     for(i = 0;i<A.n; i++){
         for (j = 0; j<A.m ; j++){
            Aux.matriz[i][j] = A.matriz[i][j]*b;
         }
     }
     //mostrarMatriz(Aux);
     cleanMatriz(B);
     B->matriz = inicializarMatrizMem(Aux.n,Aux.m);
     B->n = Aux.n;
     B->m = Aux.m;
     for(i = 0; i<Aux.n; i++){
         for(j = 0; j<Aux.m; j++){
            B->matriz[i][j] = Aux.matriz[i][j];
         }
     }
   }else{
     printf("Matrices no validas\n");
   }
}

int numPlaces (int n) {
    if (n < 0) {n = n*(-1);}
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    return 10;
}

int tam_matriz_string(Matrix A){
   int size = 0;
   int i; int j;
   for(i = 0; i<A.n; i++){
     for(j = 0; j<A.m; j++){
       size += numPlaces(A.matriz[i][j]);
     }
   }

   size = size + (A.n * A.m);
   size += (numPlaces(A.n)+1);
   size += (numPlaces(A.m)+1);
   size += (2);
   return size;
}

char *MatrizToS(Matrix A){
    char *buffer = (char*)malloc(sizeof(char)*tam_matriz_string(A));   
    char digitos[11];
    strcpy(buffer, "");
    snprintf(digitos, 11, "%d", A.n); 
    strcat(buffer, digitos); strcat(buffer,"\n");
    snprintf(digitos, 11, "%d", A.m);
    strcat(buffer, digitos); strcat(buffer,"\n");
    int i; int j;
    for(i = 0; i< A.n; i++){
        for(j = 0; j<A.m; j++){
            snprintf(digitos,11,"%d",A.matriz[i][j]);
            strcat(buffer, digitos);
            if(j != A.m-1){
               strcat(buffer," ");
            }
        }
        strcat(buffer,"\n");
    }
    //printf("%s\n", buffer);
    return buffer;
}

char *MatrizToS2(Matrix A, Matrix B){
      int sizetA = 0;
      int sizetB = 0;
      sizetA = strlen(MatrizToS(A));
      sizetB = strlen(MatrizToS(B));
      char *buffer2ble = (char *)malloc(sizeof(char)*(sizetA+sizetB+1));
      perror("Error Matriz muy Grande");
      strcpy(buffer2ble,"");
      strcat(buffer2ble,MatrizToS(A));
      strcat(buffer2ble,",");
      strcat(buffer2ble,MatrizToS(B));
      //printf("%s\n", buffer2ble);
}

void SToMatriz(char *buffer, Matrix *A){
    int i;
    int j;
    char *p;
    int l;

    p = strtok(buffer,"\n");
    int n = atoi(p);
    printf("%d\n",n);
    p = strtok(NULL,"\n");
    int m = atoi(p);
    printf("%d\n",m);

    //printf("%d,%d\n",n,m); 
    
    A->matriz = inicializarMatrizMem(n,m);
    A->n = n;
    A->m = m;
    
    //printf("AQUI\n");

    l = 0;
    for(i = 0; i<n; i++){
       for(j = 0; j<m; j++){
           if(j < m-1){
            p = strtok(NULL," ");
           }else{
            p = strtok(NULL,"\n");
           }    
           A->matriz[i][j] = atoi(p);
           l++; 
       }
       //p = strtok(NULL, "\n");
    }    
}

void ATransToB(Matrix A, Matrix *B){
  int i;
  int j;
  B->matriz = inicializarMatrizMem(A.m,A.n);
  B->n = A.m;
  B->m = A.n;
  
  printf("%d,%d\n",B->n,B->m);

  for(i = 0; i<A.n; i++){
    for(j = 0; j<A.m; j++){
        B->matriz[j][i] = A.matriz[i][j];
    }
  }

}

void AmulBToC(Matrix A, Matrix B, Matrix *C){ 

  int n = A.n;
  int m = B.m;
  printf("%d,%d\n",n,m);
  printf("A.n%d,A.m%d\n", A.n, A.m);
  printf("B.n%d,B.m%d\n", B.n, B.m);

  if(A.m != B.n){
    printf("Error matrices no compatibles M1(nxm) y M2(mxp)\n");
    exit(1);
  }


  C->matriz = inicializarMatrizMem(n,m);
  C->n = n;
  C->m = m;

  int i; int j; int suma; int k;

  for (i = 0 ; i < n ; i++ ) {
      for (k = 0 ; k < m ; k++ ){
          suma = 0;
          for (j = 0 ; j < A.m ; j++ ) {
              suma = suma + (A.matriz[i][j]*B.matriz[j][k]);
              C->matriz[i][k] = suma;
          }
      }
  }
}



int main(int argc, char *argv[]){
 
 Matrix A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,AUX;
 inicializarTodasMatrizMem((Matrix *[]){&A,&B,&C,&D,&E,&F,&G,&H,&I,&J,&K,&L,&M,&N,&O,&P,&Q,&R,&S,&T,&U,&V,&W,&X,&Y,&Z,&AUX});

 /*fileToMatriz(&A,"A.txt",'A');
 mostrarMatriz(A);
 matrizToFile(&A,"Aux.txt", 'A');
 fileToMatriz(&B,"Aux.txt", 'B');
 mostrarMatriz(B);
 cleanMatriz(&A);
 mostrarMatriz(A);
 MaToMa(&A,&B);
 mostrarMatriz(A);
 cleanMatriz(&B);
 mostrarMatriz(A);
 ASumBToC(A,B,&C);
 MaToMa(&B,&A);
 ASumBToC(A,B,&C);
 mostrarMatriz(C);
 ASumBToC(A,B,&A);
 mostrarMatriz(A);
 AResBToC(A,B,&A);
 mostrarMatriz(A);
 AmulbToB(A,3,&N);*/
 //mostrarMatriz(N);
 //fileToMatriz(&Z,"Z.txt",'Z');

 //printf("%d\n", numPlaces(10000));
 //printf("%d\n", tam_matriz_string(A));

 //ASumBToC(A,A,&C);
 //MatrizToS(A);
 //MsatrizToS2(A,C);
 //printf("%s\n", MatrizToS(A));

 //SToMatriz(MatrizToS(C),&A);
  
 //mostrarMatriz(A);
 
 
 //ATransToB(A,&A);
 
 //mostrarMatriz(Z);
 //mostrarMatriz(A);
 
 //printf("%s",MatrizToS(Z));
 //printf("%s",MatrizToS2(Z,A));

 fileToMatriz(&A,"A.txt",'A');
 
 AmulBToC(A,Z,&G);
 mostrarMatriz(G);

 return 0;
}
