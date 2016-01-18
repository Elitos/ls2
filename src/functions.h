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

int getTam_lista (int * l);

int * put_elemento( int * l, int a);

void find_delete(int * l, int a);

void delete_element(int * l, int i);

int Getpivot(int *array, int I, int D);

void Qsort(int *array, int I, int D);

int find_min_size(int ** l, int nl);

int find_element(int *l, int a);
