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
int archivo_salida(int nGrupos, int nHebras, int nListas,int **s_grupo , int nHebras_total, hebra * arrhebras){
	int i; int j;
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
	int ID_primero = 0; int ID_segundo = 0; int ID_tercero = 0;

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
}