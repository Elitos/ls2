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

#include "functions.h"

int getTam_lista (int * l) {
    int z = 0; int cont=0;
	while(l[z] != '\0'){
	  cont++; z++;
	}
	return cont;
}

int * put_elemento( int * l, int a){
  int Taml = getTam_lista(l); Taml+=2;
  int * t = l;
  int * t2 = (int *) malloc((Taml)* sizeof(int)); int k;
  for( k = 0 ; k < getTam_lista(t) ; k++){
    t2[k] = t[k];
  }
  t2[k]= a; t2[k+1]='\0';
  l = t2; free(t);
  return l;
}

void find_delete(int * l, int a){
	int t;
	int indice_elem; 
	int is = 0;
	int Taml = getTam_lista(l);
	for(t= 0 ; t < Taml ; t++){
	if(l[t]== a){
	  indice_elem = t;
	  is = 1;
	}
	}
	if(is==0){}
	else{
	Taml--;
	for( t = indice_elem ; t < Taml ; t++){
	  l[t]= l[t+1];
	}
	l[Taml]= '\0';
	}
}

void delete_element(int * l, int i){
  int t;
  int Taml =getTam_lista(l);
  if( i >= Taml || i < 0 ) {
  	printf("Invalida posicion.\n");
  }else{
    Taml--;
    for(t=i;t<Taml;t++){ l[t]= l[t+1]; }
    l[Taml]= '\0';
  }
}

int Getpivot(int *array, int I, int D){
    int t; int p; int v_p; int aux;
    p = I; v_p = array[p];
    for (t=I+1; t<=D; t++){
     if (array[t] < v_p){ p++; aux=array[t]; array[t]=array[p]; array[p]=aux; }
    }
    aux=array[I]; array[I]=array[p]; array[p]=aux;
    return p;
}

void Qsort(int *array, int I, int D){
     int p;
     if(I < D){
        p = Getpivot(array, I, D);
        Qsort(array, I, p-1); Qsort(array, p+1, D);
     }
}

int find_min_size(int ** l, int tamLs){
  int minTam = getTam_lista(l[0]); int lmenor; int t;
  for( t = 1 ; t < tamLs ; t++){
    int v = getTam_lista(l[t]);
    if(v < minTam){ minTam = v; lmenor = t; }
  }
  return lmenor;
}

int find_element(int *l, int elem){
  int t;
  for(t=0;t<getTam_lista(l);t++){
    if(l[t]== elem) return 1;

  }
  return 0;
}
