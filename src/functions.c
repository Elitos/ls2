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

#include "functions.h"

int getLine(char *prmpt, char *buff, size_t sz) {
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

int isNumeric(const char *str) 
{
    while(*str != '\0')
    {
        if(*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
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
       if(array[1][0] >= 65 && array[1][0] <= 90 && strlen(array[1]) < 2){ // load o save A
          removeChar(array[2],'"');
          //printf(" array sin \" %s\n",array[2]); // load o save A "A.txt"
          //if(array[2][0] >= 65 && array[2][0] <=90 && array[2][1] == '.' && (strstr(array[2], ".txt")) != NULL && array[2][2] == 't' && array[2][3] == 'x' && array[2][4] == 't'){
          if(array[2][0] >= 65 && array[2][0] <=90 && (strstr(array[2], ".txt")) != NULL){
             return 1;
          }
       }
      }
      if(array[0][0] >= 65 && array[0][0] <= 90 && strlen(array[0]) < 2){ // A
         if(array[1][0] == '='){ //A =
            if(array[2][0] >= 65 && array[2][0] <= 90 && strlen(array[2]) < 2){ // A = B
             return 1;
            }
         }
      }  
    }

    if(nargs == 2){
      if(strcmp(array[0],"print") == 0 || strcmp(array[0],"clear") == 0){ // print o clear
       if(array[1][0] >= 65 && array[1][0] <= 90 && strlen(array[1]) < 2){ // print o clear A
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
        if(strlen(array[0])==1 && array[0][0] >= 65 && array[0][0] <= 90){ // tengo el A
            if(strlen(array[1])==1 && array[1][0] == '='){
                if(strlen(array[2])==1 && array[2][0] >= 65 && array[0][0] <= 90){ // reconosco A = A
                   if((strlen(array[3]) == 1) && (array[3][0] == '*')){ //reconosco A = A *
                      if((strstr(array[4], ".")) == NULL && (strstr(array[2], ",")) == NULL){
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
            if(array[4][0] >= 65 && array[4][0] <= 90){
               return 9;
            }
            int val = atoi(array[4]);
            if(val != 0 || array[4][0] == '0' || array[4][0] == '-' ){
            int len = strlen(array[4]); 
                return 8;
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
     //printf("Matrices no validas\n");
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

void AmulbcToB(Matrix A, int b, Matrix *B){
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
    char *buffer = (char*)malloc(sizeof(char)*tam_matriz_string(A)*10);   
    char digitos[11];
    strcpy(buffer, "");
    sprintf(digitos, "%d", A.n); 
    strcat(buffer, digitos); strcat(buffer,"\n");
    sprintf(digitos, "%d", A.m);
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
      //perror("Error Matriz muy Grande");
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
    //printf("%d\n",n);
    p = strtok(NULL,"\n");
    int m = atoi(p);
    //printf("%d\n",m);

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
  //printf("%d,%d\n",n,m);
  //printf("A.n%d,A.m%d\n", A.n, A.m);
  //printf("B.n%d,B.m%d\n", B.n, B.m);

  if(A.m != B.n){
    perror("Error matrices no compatibles M1(nxm) y M2(mxp)\n");
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
