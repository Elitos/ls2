#DIR_OUTPUT
#DIR_SRC
#NAME_EXEC
main: src/main.o src/functions.o
	@gcc -o build/main -I src/ src/main.o src/functions.o 
	@rm src/*.o
	@echo "Compilacion realizada"
	
build/main.o: src/main.c
	@gcc -o src/main.o src/main.c -c

build/functions.o: src/functions.c
	@gcc -o src/functions.o src/functions.c -c
