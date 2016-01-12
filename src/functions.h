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
#define NUM_MATRIZ 26

typedef struct $Matris{
  char name;
  int n;
  int m;
  int **matriz;
}Matrix;

// Padre

int getLine(char *prmpt, char *buff, size_t sz);

int getComando(char *comando, char *array[]);

void removeChar(char *str, char garbage);

int isComandoP_O_H(char *array[], int nargs);

int getFuncion(char *array[], int nargs);

// Hijo y Padre 

int **inicializarMatrizMem(int n, int m);

void inicializarTodasMatrizMem(Matrix *arr[]);

void mostrarMatriz(Matrix M);

void removeChar(char *str, char garbage);

int getComandos(char *comando, char *array[], int nargs);

void fileToMatriz(Matrix * M, char *filename, char name_matriz);

void matrizToFile(Matrix *M, char *filename, char name_matriz);

void cleanMatriz(Matrix *M);

void MaToMa(Matrix *destino, Matrix *inicio);

int validarSumaResta(Matrix A, Matrix B);

void ASumBToC(Matrix A, Matrix B, Matrix *C);

void AResBToC(Matrix A, Matrix B, Matrix *C);

void AmulbToB(Matrix A, int b, Matrix *B);

int numPlaces (int n);

int tam_matriz_string(Matrix A);

char *MatrizToS(Matrix A);

char *MatrizToS2(Matrix A, Matrix B);

void SToMatriz(char *buffer, Matrix *A);

void ATransToB(Matrix A, Matrix *B);

void AmulBToC(Matrix A, Matrix B, Matrix *C);
