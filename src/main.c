/*
LABORATORIO 2 SISTEMAS OPERATIVOS 2-2015
Integrantes
Elías Gonzalez 18.248.829-1
Prof: Fernando Rannou - Ayudante: Luis Loyola
*/

/*
Header donde se declaran las funciones c utilizar por el proceso principal (padre), con sus respectivos
includes c las librerias necesarias.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include <time.h>
#include "functions.h"

int nHebras; 
int nGrupos;
int nListas;
int **s_grupo; // Listas s_grupo para cada grupo.
int **s_prima; // Grupos s primas para cada grupo
int **matriz;  // Total de listas 
int * noSeEncuentran; // Elementos de S
double * tiempos_x_grupo; // Guarda los tiempos por grupo, segun index
pthread_mutex_t  * arrMutex; // Array mutex por grupo y lectura
pthread_mutex_t t_mutex = PTHREAD_MUTEX_INITIALIZER; // PARA FINALIZAR EL CLOCK

pthread_barrier_t barrera_inicial;  // Todas partan al mismo tiempo
pthread_barrier_t * arr_barrera_1_a_1; // Todas lean u no se adelante ninguna hebra
pthread_barrier_t * arr_barrera_1_a_M; // Todas accedan a la matriz a la vez.
pthread_barrier_t * arr_barrera_1_a_sprima; // Todas modifiquen sprima en una sola ocacion
pthread_barrier_t * arr_barrera_1_s_grupo; // Los grupos esperen a que otras hebras terminen de procesar
pthread_barrier_t * arr_barrera_1_a_gs; // Las hebras esperen a que todas modifiquen s'


void *funcionHebras(void *args) {
  int cont_t = 0;
  int arg =  (int)args;
  int Hebra_ID = arrhebras[arg].nHiloG;
  int Grupo_ID = arrhebras[arg].nGrupoG;

  arrhebras[arg].t_x2[0] = (double *) malloc((nListas)* sizeof(double));
  arrhebras[arg].t_x2[1] = (double *) malloc((nListas)* sizeof(double));

  pthread_barrier_wait(&barrera_inicial); // barrera todos a la vez
  clock_t t_init_h = clock(); // Tiempo por grupo todas las hebras
  arrhebras[arg].t_init = t_init_h;

  int i; int j; int k;
  int indice_desde = 0; // Desde donde lee cada hebra
  int indice_hasta = 0;  // Hasta donde debe leer la hebra
  int Tam_parte_s; // Tamaño de la parte de S
  int amplitud_sub_lista;
  int Tam_lista; // Tamño de la con la cual se compara
  
  for( i = 0 ; i < nListas ; i++){//para todas las listas
    pthread_barrier_wait(&arr_barrera_1_a_M[Grupo_ID]); // Hebras sobre la misma lista
    clock_t t_hebra_individual = clock(); // Tiempo por hebra individual
    arrhebras[arg].t_x2[0][cont_t] = t_hebra_individual;
    pthread_mutex_lock(&arrMutex[Grupo_ID]); // Lock de matriz a leer hebra por grupo una a la vez.
    Tam_lista = getTam_lista(matriz[i]);
    pthread_mutex_unlock(&arrMutex[Grupo_ID]);
    Tam_parte_s = Tam_lista/nHebras; // Calculo tamaño de la parte
    indice_desde = Tam_parte_s*Hebra_ID; // Desde donde debe leer esta hebra
    if(Hebra_ID == (nHebras-1)){ // Ultima hebra de cada grupo puede leer menos del total
      indice_hasta = Tam_lista-1;
    }else{
      indice_hasta = indice_desde + Tam_parte_s -1;
    }
    pthread_mutex_lock(&arrMutex[Grupo_ID]); // Leer elemento de lista
    int tam_lista_s = getTam_lista(s_grupo[Grupo_ID]);
    pthread_mutex_unlock(&arrMutex[Grupo_ID]);

    amplitud_sub_lista = indice_hasta - indice_desde +1;
    if(amplitud_sub_lista > 1){ // Largo de la lista necesita reordenar
      pthread_mutex_lock(&t_mutex);
      Qsort(matriz[i],indice_desde,indice_hasta);
      pthread_mutex_unlock(&t_mutex);
    }
    pthread_barrier_wait(&arr_barrera_1_a_sprima[Grupo_ID]);
    if(Hebra_ID==0){
      pthread_mutex_lock(&arrMutex[Grupo_ID]);
      pthread_mutex_lock(&t_mutex);
      int tam_lista_sprima = getTam_lista(s_prima[Grupo_ID]);
      for( j = 0 ; j < tam_lista_sprima ; j++){ 
        delete_element(s_prima[Grupo_ID],0);
      }
      pthread_mutex_unlock(&t_mutex);
      pthread_mutex_unlock(&arrMutex[Grupo_ID]);
    }
    pthread_barrier_wait(&arr_barrera_1_s_grupo[Grupo_ID]);
    int indice;
    for( j = 0 ; j < tam_lista_s ; j++){ //para cada elemento de s_grupo
      indice= indice_desde;
      while(indice<=indice_hasta){
        pthread_mutex_lock(&arrMutex[Grupo_ID]);  
        if (s_grupo[Grupo_ID][j]==matriz[i][indice]){
          s_prima[Grupo_ID] = put_elemento(s_prima[Grupo_ID],matriz[i][indice]);
        }
        pthread_mutex_unlock(&arrMutex[Grupo_ID]);
        indice++;
      }
    }
    pthread_barrier_wait(&arr_barrera_1_a_gs[Grupo_ID]);
    pthread_mutex_lock(&arrMutex[Grupo_ID]);
    pthread_mutex_lock(&t_mutex);
    
    if(Hebra_ID==0){
      int tam_s = getTam_lista(s_grupo[Grupo_ID]);
      for( j = 0 ; j < tam_s ; j++){
        if(find_element(s_prima[Grupo_ID],s_grupo[Grupo_ID][j])!=1){
          find_delete(s_grupo[Grupo_ID],s_grupo[Grupo_ID][j]); 
        }
      }
    }
    pthread_mutex_unlock(&t_mutex);
    pthread_mutex_unlock(&arrMutex[Grupo_ID]);
    clock_t time_hebra_inicial = clock();
    arrhebras[arg].t_x2[1][cont_t] = time_hebra_inicial;
    cont_t++;
    pthread_barrier_wait(&arr_barrera_1_a_1[Grupo_ID]);
  }
  clock_t tiempoFinHebra = clock();
  arrhebras[arg].t_end = tiempoFinHebra;
}

int main(int argc, char ** argv) {
  char *SnHebras; char *SnGrupos; char *File_name; char opcion;
  while((opcion = getopt(argc, argv,"g:h:i:"))!= -1){
    if(opcion == -1){
        printf("Entradas no validas\nFormato entrada: ./main -i entrada.dat -g 5 -h 4\n");
        abort();
    }
    switch (opcion){
      case 'g' : 
        SnGrupos = optarg;
        break;
      case 'h' : 
        SnHebras = optarg;
        break;
      case 'i' : 
        File_name = optarg;
        break;
      default : /*Errores*/
        printf("Entradas no validas\nFormato entrada: ./main -i entrada.dat -g 5 -h 4\n");
        abort();
    }
  }
  nGrupos = atoi(SnGrupos); nHebras = atoi(SnHebras); 
  if(nGrupos < 1 || nHebras < 1){
     printf("Entrada no valida, hebras y grupos\n como minimo valor debe ser 1\n");
     abort();
  }
  matriz = (int **)malloc(sizeof (int**));
  matriz[0]= (int *)malloc(sizeof (int*));
  FILE * archivo_entrada =  fopen(File_name,"r");
  int i; int j;
  int cX = 1; int cY = 1;
  char c = 'c';
  int numeroElemListas = 0;
  int elemento = 0; nListas = 1; int cl = 0;

  int nMinL;  int largoMinL;

  while(c != EOF){
    fscanf(archivo_entrada,"%d",&numeroElemListas);
    matriz[cX-1]= (int *)realloc(matriz[cX-1],(numeroElemListas)*sizeof(int*));
    for( i = 1 ; i <= numeroElemListas ; i++){         
      fscanf(archivo_entrada,"%d",&elemento);
      matriz[cX-1][i-1] = elemento;
      cY++;
    }
    cl++;
    if(cl == 1 || largoMinL > numeroElemListas){
      largoMinL = numeroElemListas;
      nMinL = cl;
    }
    c = fgetc(archivo_entrada);//*********matriz[cX-1]= (int *)realloc(matriz[cX-1], cY * sizeof(int*) );///matriz[cX-1][cY-1] = '\0';      
    if(c != EOF){
      nListas++; cX++;
      matriz = (int **)realloc(matriz,cX*sizeof(int**));
      matriz[cX-1]= (int *)malloc(sizeof(int));
      cY = 1;            
    }
  }
  fclose(archivo_entrada);


  // Matriz menor largo
  nMinL = nMinL-1;
  //printf("El largo de la menor lista es: %i y es la lista número %i.\n", largoMinL, nMinL +1);  
  int * s_incial = (int *)malloc(sizeof(int)); s_incial = matriz[nMinL];

  int faltan = nMinL+1;
  for(faltan ; faltan < nListas ; faltan++){
      matriz[faltan-1]= matriz[faltan]; 
  }
  matriz[nListas-1]= (int *)malloc(sizeof(int));
  free(matriz[nListas-1]);
  nListas--;

  // Grupo de listas S prima para caga grupo de hebras
  s_grupo = (int **)malloc(sizeof(int*)*nGrupos);
  for( i = 0 ; i < nGrupos ;i++){ 
    s_grupo[i]= (int*) malloc(largoMinL* sizeof(int)); s_grupo[i] = s_incial;
  }    

  // Para el total de hebras en competencia creo un s' nulo de tamaño igual al de menor lista
  int nHebras_total = nHebras * nGrupos; 
  tiempos_x_grupo = (double*)malloc(nGrupos*sizeof(double));
  arrhebras = (hebra*)malloc(nHebras_total* sizeof(hebra)); 
  s_prima = (int**)malloc(nGrupos*sizeof(int*));
  for( i = 0 ; i < nGrupos ; i++){
   s_prima[i] = (int*)malloc(largoMinL* sizeof(int)); s_prima[i][0]= '\0';
  }

  // Creo array de mutex paa accesso de cada hebra a su s comun por grupo
  arrMutex = (pthread_mutex_t*)malloc(nGrupos*sizeof(pthread_mutex_t));
  for(i = 0 ; i < nGrupos ; i++){
    pthread_mutex_init(&arrMutex[i],NULL);
  }
  // Relaciono hebras con grupos
  int ch = 0;
  for (i = 0; i < nGrupos; i++) {
    for( j = 0 ; j < nHebras ; j++){ arrhebras[ch].nGrupoG = i; arrhebras[ch].nHiloG = j; ch++; } 
  }
  
  // creo barreras coordinacion
  pthread_barrier_init(&barrera_inicial, NULL, nHebras_total); noSeEncuentran = (int *)malloc(largoMinL* sizeof(int)); noSeEncuentran[0] = '\0';
  arr_barrera_1_a_1 = (pthread_barrier_t *)malloc(nGrupos*(sizeof(pthread_barrier_t))); arr_barrera_1_a_M = (pthread_barrier_t *)malloc(nGrupos*(sizeof(pthread_barrier_t)));
  arr_barrera_1_a_sprima = (pthread_barrier_t *)malloc(nGrupos*(sizeof(pthread_barrier_t))); arr_barrera_1_s_grupo = (pthread_barrier_t *)malloc(nGrupos*(sizeof(pthread_barrier_t)));
  arr_barrera_1_a_gs = (pthread_barrier_t *)malloc(nGrupos*(sizeof(pthread_barrier_t)));

// Instacio barreras
  for( i = 0 ; i < nGrupos ; i++){
    pthread_barrier_init(&arr_barrera_1_a_1[i], NULL, nHebras); pthread_barrier_init(&arr_barrera_1_a_M[i], NULL, nHebras);
    pthread_barrier_init(&arr_barrera_1_a_sprima[i], NULL, nHebras); pthread_barrier_init(&arr_barrera_1_s_grupo[i], NULL, nHebras);
    pthread_barrier_init(&arr_barrera_1_a_gs[i], NULL, nHebras);
  }
  //Arranco hebras 
  for( i = 0 ; i < nHebras_total; i++){
    pthread_create(&arrhebras[i].thread_h, NULL, funcionHebras, (void*)i);
  }
  // Espero hebras
  for (i = 0 ; i < nHebras_total; i++) {
    pthread_join(arrhebras[i].thread_h, NULL); 
  }
  // Genero archivo
  archivo_salida(nGrupos,nHebras, nListas, s_grupo,nHebras_total,arrhebras);

}



