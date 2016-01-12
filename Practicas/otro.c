#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LEER 0
#define ESCRIBIR 1

int main (int argc, char *argv[]) { 

 char *buff = (char*)malloc(sizeof(char)*1000);

 read(0,buff,1000);
 //Recivo matriz  con formato string parseo a matriz luego realizo operacion
 // y la mando por write (matriz resultado la parseo a string)
 
 strcat(buff,"\nMensaje de hijo a padre");

 write(1,buff,1000);

 return 0;
}