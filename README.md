Laboratorio 2 Sistemas Operativos.
Integrantes:
- Elías González M - 18.674.643-0

Observaciones:
- Para archivos muy grandes en tamaño, esperar unos minutos.
- Los archivos que contiene las listas deben ser .dat y deben encontrarce en la carpeta build.
- Los elementos de las listas deben estar ordenados de menor a mayor.

En el archivo se incluyen:
- La carpeta build, que se llena con los ejecutables despues de la compilación y los archivos de prueba.
- La carpeta src, que contiente los códigos del programa: main.c (programa padre principal), los headers functions.h (con su functions.c correspondiente) que contiene gran parte de las funciones que se utilizan por padres e hijos. 
 * main.c (programa principal que ejecuta proceso principal)
- makefile para la compilación del programa
- Este archivo readme.

**********************************************************
La finalidad de éste archivo es explicar paso a paso el proceso de compilación del programa.
Para compilar es necesario utilizar la terminal de Linux, y seguir los siguientes pasos:
- Abrir el directorio donde se ha descomprimido el archivo, utilizando el comando "cd", por ejemplo ~$ cd Escritorio/Laboratorio 2 Sistope
- Verificar que los archivos descritos anteriormente se encuentren en dicho directorio
- Escribir el comando "make" en la terminal.

Una vez realizados los pasos anteriormente descritos, el programa se compilara exitosamente.

Pasos para su ejecución.

Para ejemplificar el uso del programa, buscaremos la palabra "build" en el directorio /Laboratorio 2 Sistope/build/*.

En la terminal de linux, para ejecutar el programa importa el orden en el cual se ingresen los parametros. Para ejecutar el programa estando en build, ejecutamos  "./main -i entrada.dat -g 5 -h 4" por ejemplo.

********Cabe destacar nuevamente que el archivo entrada.dat se debe encontrar en la carpeta build.

