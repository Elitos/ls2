/*
LABORATORIO 2 SISTEMAS OPERATIVOS 2-2015
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

typedef struct $hebra{ // Estructura que soporta la hebra y su funcionalidad
  int nGrupoG; // Numero del grupo al cual pertenece la hebra
  int nHiloG; // Numero del hilo al que pertenece la hebra (ID_HEBRA)
  float t_init; // tiempo de iniciacion del grupo
  float t_end; // Tiempo de fin del grupo
  float t_all; // Tiempo de finalizacion de todas las lecturas de listas
  double * t_x2[2]; // Para calculos numericos // Tiempos de entrada y salida
  pthread_t thread_h; // Hebra para ejecutar e instanciar
}hebra;
hebra * arrhebras;

int getTam_lista (int * l); // Retorna el tamaño de la lista l
int * put_elemento( int * l, int a); // Coloca un elemento en la lista l
void find_delete(int * l, int a); // Encuentra un elemento en la lista l
int Getpivot(int *array, int I, int D); // Obtiene pivote para qsort
void delete_element(int * l, int i); // Borra elemento de la lista
void Qsort(int *array, int I, int D); // Ordena arreglo con qsort
int find_min_size(int ** l, int nl); // Encuentra la lista de tamaño minimo
int find_element(int *l, int a); // Encuentra elemento en la lista
int archivo_salida(int nGrupos, int nHebras , int nListas, int **s_grupo ,int nHebras_total,hebra * arrhebras);
// Genera archivo de salida u calcula lugares con matriz de arrhebras. (cada hebra tiene su tiempo)