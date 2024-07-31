#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "first_pass.h"
#include "table.h"
#include "utils.h"
#include "globals.h"

/**
 * The main function of the assembly-to-machine code program.
 * It processes the input assembly files provided via command line arguments.
 * @param argc Count of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Success status of file processing.
 */
int main(int argc, char *argv[])
{
	int fileSuccess = 0, i;
	if (argc == 1) /*if there is no file in the input*/
	{
		printf("ERROR: Please enter asseembly file name");
		exit(EXIT_SUCCESS); 
	}

	for(i = 1; i < argc; i++) /*passing over the files in the input*/
	{
		fileSuccess = assembly_to_machine_code(argv[i]);
		if(fileSuccess == 0)
		{
			printf("\n '%s' The file is failed.\n", argv[i]);
		}
	}	

return fileSuccess;
}

/**
 * Transforms the provided assembly file into machine code.
 * This function invokes the pre-assembler, first pass, and second pass functions.
 * @param fileName The name/path of the assembly file to process.
 * @return Success status of assembly to machine code conversion.
 */
int assembly_to_machine_code(char* fileName)
{	long ic = 0, dc = 0;
	int success = 0, line_number = 0, create_entry_file = 1, create_extern_file = 1;
	char *currLine = (char*)calloc(MAX_LENGTH_LINE + 2, sizeof(char));
	char *amFileName = (char*)malloc(256 * sizeof(char));
	entry_symbols **head_entry;
	extern_symbols **head_extern;

	FILE *file_dest;
	SymbolTable *symbolTable = NULL;
	
	/* Pre-assemble the input file */
	success = preassembler_file(fileName);
	if (success != 1) {
		printf("couldn't spread the macros correctly into %s.as\n", fileName);
	}

	/* Open the file with the .am suffix */
	amFileName = filename_suffix(fileName, ".am");
	file_dest = fopen(amFileName, "r");
	if (file_dest == NULL) {
		handle_error(ERROR_FILE_READ, line_number);
		free(amFileName);
		return 0;
	}

	/*starting first pass*/
	symbolTable = create_symbol_table(); /*we storage the symbols here*/

	while (fgets(currLine, MAX_LENGTH_LINE + 2, file_dest) != NULL) { /*go over every line in the file*/
		if (strchr(currLine, '\n') == NULL && !feof(file_dest)) {
			handle_error(ERROR_LINE_LENGTH, line_number); /*the length of line is too long*/
			success = 0;
		}
		if (!currLine) {
			handle_error(ERROR_MEMORY_ALLOCATION, line_number);
		}
		else {
			success = first_pass(file_dest, currLine, symbolTable, &ic, &dc, line_number);
		}
		line_number++;
	}
	
	/*starting the second pass*/
	rewind(file_dest);
	if (success) {
		success = second_pass(file_dest);
	}

	if (success) { /*print the files*/
		create_files(head_entry, head_extern, create_entry_file, create_extern_file);
	}
			
	free(currLine);	
	free(file_dest);
	free(symbolTable);

	
	return success;
}
