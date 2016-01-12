#DIR_OUTPUT
#DIR_SRC
#NAME_EXEC
main: src/main.o src/functions.o src/functionschildSum.o src/functionschildRes.o src/functionschildMulC.o src/functionschildMul.o src/functionschildTran.o
	@gcc -o build/main -I src/ src/main.o src/functions.o 
	@gcc -o build/childSum -I src/ src/functionschildSum.o src/functions.o
	@gcc -o build/childRes -I src/ src/functionschildRes.o src/functions.o
	@gcc -o build/childMulC -I src/ src/functionschildMulC.o src/functions.o
	@gcc -o build/childMul -I src/ src/functionschildMul.o src/functions.o
	@gcc -o build/childTran -I src/ src/functionschildTran.o src/functions.o
	@rm src/*.o
	@echo "Compilacion realizada"
	
build/main.o: src/main.c
	@gcc -o src/main.o src/main.c -c

build/functions.o: src/functions.c
	@gcc -o src/functions.o src/functions.c -c

build/functionschildSum.o: src/functionschildSum.c
	@gcc -o src/functionschildSum.o src/functionschildSum.c -c

build/functionschildRes.o: src/functionschildRes.c
	@gcc -o src/functionschildRes.o src/functionschildRes.c -c

build/functionschildMulC.o: src/functionschildMulC.c
	@gcc -o src/functionschildMulC.o src/functionschildMulC.c -c

build/functionschildMul.o: src/functionschildMul.c
	@gcc -o src/functionschildMul.o src/functionschildMul.c -c

build/functionschildTran.o: src/functionschildTran.c
	@gcc -o src/functionschildTran.o src/functionschildTran.c -c