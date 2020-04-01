assembler: assembler.o instructions.o list.o types.h
	gcc -g assembler.o instructions.o list.o -ansi -pedantic -Wall -o assembler
assembler.o: assembler.c types.h
	gcc -c assembler.c -ansi -pedantic -Wall -o assembler.o
list.o: list.c types.h
	gcc -c list.c -ansi -pedantic -Wall -o list.o
instructions.o: instructions.c types.h
	gcc -c instructions.c -ansi -pedantic -Wall -o instructions.o
