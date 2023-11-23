CC = gcc # GCC Compiler
CFLAGS = -ansi -Wall -pedantic # Flags
GLOBALS_FILES = globals.h 
ALL_OBJ =  utils.o table.o preassembler.o errors.o first_pass.o binary.o second_pass.o second_pass_functions.o

assembler: assembler.h $(ALL_OBJ) $(GLOBALS_FILES)
	$(CC) -g assembler.c $(CFLAGS) $(ALL_OBJ) -o $@
	
assembler.o: assembler.c $(GLOBALS_FILES)
	$(CC) -c assembler.c $(CFLAGS) -o $@

preassembler.o: preassembler.c preassembler.h $(GLOBALS_FILES)
	$(CC) -c preassembler.c $(CFLAGS) -o $@


table.o: table.c table.h $(GLOBALS_FILES)
	$(CC) -c table.c $(CFLAGS) -o $@

utils.o: utils.c utils.h $(GLOBALS_FILES)
	$(CC) -c utils.c $(CFLAGS) -o $@

first_pass.o: first_pass.c first_pass.h binary.h $(GLOBALS_FILES)
	$(CC) -c first_pass.c $(CFLAGS) -o $@

binary.o: binary.c binary.h $(GLOBALS_FILES)
	$(CC) -c binary.c $(CFLAGS) -o $@

errors.o: errors.c utils.h $(GLOBALS_FILES)
	$(CC) -c errors.c $(CFLAGS) -o $@

second_pass.o: second_pass.c utils.h $(GLOBALS_FILES)
	$(CC) -c second_pass.c $(CFLAGS) -o $@

second_pass_functions.o: second_pass_functions.c utils.h $(GLOBALS_FILES)
	$(CC) -c second_pass_functions.c $(CFLAGS) -o $@


