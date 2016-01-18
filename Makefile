#DIR_OUTPUT
#DIR_SRC
#NAME_EXEC
main: src/main.o src/functions.o 
	@gcc -o build/main  -lpthread -I src/ src/main.o src/functions.o -lpthread
	@rm src/*.o
	@echo "Compilacion realizada con exito"
	
build/main.o: src/main.c
	@gcc -o src/main.o src/main.c -c

build/functions.o: src/functions.c
	@gcc -o src/functions.o src/functions.c -c
