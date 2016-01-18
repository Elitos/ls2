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

typedef struct $hebra{
  int nGrupoG;
  int nHiloG;
  float t_init;
  float t_end;
  float t_all;
  double * t_x2[2]; // Para calculos numericos
  pthread_t thread_h;
}hebra;
hebra * arrhebras;

pthread_barrier_t barrera_inicial;  // Todas partan al mismo tiempo
pthread_barrier_t * arr_barrera_1_a_1; // Todas lean u no se adelante ninguna hebra
pthread_barrier_t * arr_barrera_1_a_M; // Todas accedan a la matriz a la vez.
pthread_barrier_t * arr_barrera_1_a_sprima;
pthread_barrier_t * arr_barrera_1_s_grupo;
pthread_barrier_t * arr_barrera_1_a_gs;


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

  FILE * archivo_entrada =  fopen(File_name,"r");

  matriz = (int **)malloc(sizeof (int**));
  matriz[0]= (int *)malloc(sizeof (int*));

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
    for( j = 0 ; j < nHebras ; j++){
      arrhebras[ch].nGrupoG = i; arrhebras[ch].nHiloG = j;
      ch++;
    } 
  }
  
  pthread_barrier_init(&barrera_inicial, NULL, nHebras_total);
  noSeEncuentran = (int *)malloc(largoMinL* sizeof(int)); noSeEncuentran[0] = '\0';
  arr_barrera_1_a_1 = (pthread_barrier_t *)malloc(nGrupos*(sizeof(pthread_barrier_t)));
  arr_barrera_1_a_M = (pthread_barrier_t *)malloc(nGrupos*(sizeof(pthread_barrier_t)));
  arr_barrera_1_a_sprima = (pthread_barrier_t *)malloc(nGrupos*(sizeof(pthread_barrier_t)));
  arr_barrera_1_s_grupo = (pthread_barrier_t *)malloc(nGrupos*(sizeof(pthread_barrier_t)));
  arr_barrera_1_a_gs = (pthread_barrier_t *)malloc(nGrupos*(sizeof(pthread_barrier_t)));

  for( i = 0 ; i < nGrupos ; i++){
    pthread_barrier_init(&arr_barrera_1_a_1[i], NULL, nHebras);
    pthread_barrier_init(&arr_barrera_1_a_M[i], NULL, nHebras);
    pthread_barrier_init(&arr_barrera_1_a_sprima[i], NULL, nHebras);
    pthread_barrier_init(&arr_barrera_1_s_grupo[i], NULL, nHebras);
    pthread_barrier_init(&arr_barrera_1_a_gs[i], NULL, nHebras);
  }

  for( i = 0 ; i < nHebras_total; i++){
    pthread_create(&arrhebras[i].thread_h, NULL, funcionHebras, (void*)i);
  }

  for (i = 0 ; i < nHebras_total; i++) {
    pthread_join(arrhebras[i].thread_h, NULL); 
  }

  ////////////////////////////////////////////////// Time analisis

  double  t_start = arrhebras[0].t_init;
  double  * t_lastAdd = (double *) malloc((nGrupos)* sizeof(double));
  double  * t_grupo = (double *) malloc((nGrupos)* sizeof(double));

  for(i=0;i<nHebras_total;i++){ // A todas acctualizo tiempo de inicio
    if(arrhebras[i].t_init < t_start){
      t_start = arrhebras[i].t_init;
    }
  }

  int indice = 0; // Sumatorea de tiempo grupal
  for( i = 0 ; i < nGrupos ; i++){
    t_lastAdd[i] = arrhebras[(arrhebras[indice].nGrupoG)*(nHebras)].t_end;
    for( j = 0 ; j < nHebras ; j++){     
      arrhebras[indice].t_all = arrhebras[indice].t_end-t_start;
      if(arrhebras[indice].t_end > t_lastAdd[i]){
        t_lastAdd[i] = arrhebras[indice].t_end;
      }
      indice++;
    }
    t_grupo[i]= t_lastAdd[i]-t_start;
  }

  // Lugares
  int ID_primero = 0;
  int ID_segundo = 0;
  int ID_tercero = 0;

  for ( i = 0; i < nGrupos; i++){
    if(t_grupo[i] < t_grupo[ID_primero]){ ID_primero = i;}
  }
  if(nGrupos > 1){
    while(ID_segundo == ID_primero){
      ID_segundo++;
      if(ID_segundo >= nGrupos){ ID_segundo=0; }
    }
    for ( i = 0; i < nGrupos;i++){
      if(i!=ID_primero){
        if(t_grupo[i]<t_grupo[ID_segundo]){ ID_segundo=i; }
      }
    }
  }
  if(nGrupos>2){
    while(ID_tercero==ID_primero || ID_tercero==ID_segundo){
      ID_tercero++;
      if(ID_tercero>=nGrupos){ ID_tercero=0; }
    }
    for ( i = 0; i < nGrupos ; i++){
      if(i!=ID_primero && i != ID_segundo){ 
        if(t_grupo[i]<t_grupo[ID_tercero]){ ID_tercero=i; }
      }
    }
  }

  //Calculo hebra mas eficiente
  int ID_h_eficiente = 0;
  int ID_h_eficiente_grupo = 0;
  int t_eficiente = arrhebras[0].t_all;
  for( i = 0; i < nHebras_total; i++){
    if(arrhebras[i].t_all < t_eficiente){
      t_eficiente = arrhebras[i].t_all;
      ID_h_eficiente = arrhebras[i].nHiloG;
      ID_h_eficiente_grupo =  arrhebras[i].nGrupoG;
    }
  }
  double t_hs[nHebras_total][nListas];
  for( i = 0 ; i < nHebras_total ; i++){
    for( j = 0 ; j < nListas ; j++){
      double var_temp = arrhebras[i].t_x2[1][j] - arrhebras[i].t_x2[0][j];
      t_hs[i][j] = var_temp;
    }
  }
  double t_prom_hebra[nHebras_total];
  for ( i = 0 ; i < nHebras_total ; i++){
    int t_prom_add = 0;
    for( j = 0; j < nListas ; j++){
      t_prom_add = t_prom_add + t_hs[i][j];
    }
    t_prom_hebra[i] = t_prom_add/nListas;
  }

  int ID_h_eficiente_prom = 0; 
  int ID_h_eficiente_grupo_prom = 0;
  int t_eficiente_prom = t_prom_hebra[0];

  for (i=0;i<nHebras_total;i++){
    if(t_prom_hebra[i]<t_eficiente_prom){
      t_eficiente_prom = t_prom_hebra[i];
      ID_h_eficiente_prom = arrhebras[i].nHiloG;
      ID_h_eficiente_grupo_prom =  arrhebras[i].nGrupoG;
    }
  }

  printf("Creando archivo salida...\n");
  FILE* archivo_salida = fopen("resultados.txt", "w");
  fprintf(archivo_salida,"Número del equipo que obtuvo el primer lugar: %i \nTiempo del equipo que obtuvo el primer lugar: %.100g \n",ID_primero, t_grupo[ID_primero]);
  fprintf(archivo_salida,"Número del equipo que obtuvo el segundo lugar: %i \nTiempo del equipo que obtuvo el segundo lugar: %.100g \n",ID_segundo, t_grupo[ID_segundo]);
  fprintf(archivo_salida,"Número del equipo que obtuvo el tercer lugar: %i \nTiempo del equipo que obtuvo el tercer lugar: %.100g \n",ID_tercero, t_grupo[ID_tercero]);
  fprintf(archivo_salida,"Hebra más_grupo eficiente: %d del grupo %d.\n",ID_h_eficiente, ID_h_eficiente_grupo);
  fprintf(archivo_salida,"Hebra más_grupo eficiente en promedio: %d del grupo %d.\n", ID_h_eficiente_prom,ID_h_eficiente_grupo_prom);
  fprintf(archivo_salida,"Intersección de las listas: ");
  for( i = 0; i < getTam_lista(s_grupo[0]) ; i++){
    fprintf(archivo_salida, "%i ",s_grupo[0][i]);
  }
  fprintf(archivo_salida,"\n");
  fclose(archivo_salida);
  printf("Archivo salida creado.\n");
}


