 
assembler: assembler.o helper_functions.o memory.o pre_assembler.o
	gcc -ansi -Wall -g assembler.o helper_functions.o memory.o pre_assembler.o -o assembler
helper_functions.o: helper_functions.c helper_functions.h
	gcc -c -g -ansi -Wall helper_functions.c -o helper_functions.o 
	
pre_assembler.o: pre_assembler.c pre_assembler.h
	gcc -c -g -ansi -Wall pre_assembler.c -o pre_assembler.o
	
memory.o: memory.c memory.h
	gcc -c -g -ansi -Wall memory.c -o memory.o


assembler.o: assembler.c assembler.h
	gcc -c -g -ansi -Wall assembler.c -o assembler.o