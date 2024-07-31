/**
 * This file is the second pass of our assembler project.
 * This assembler, in its second iteration, meticulously reviews the entirety of the source document,
 * refining the machine code pertaining to operands which utilize symbols by harnessing the values derived from the symbol table.
 * Upon the culmination of this secondary phase, the software will be wholly transmuted into its machine code representation.
 * In order to build the second transition, we used the algorithm in the course booklet.
 */

#include "globals.h"
#include "utils.h"

/**
 * The second_pass function carries out the secondary transition over the provided file.
 * The main aim of this pass is to interpret and convert the assembly code into machine-readable format.
 * 
 * @param file_ptr Pointer to the source assembly file.
 * @return Returns 1 upon successful execution.
 */
int second_pass(FILE *file_ptr) {
	
	char current_line[MAX_LENGTH_LINE_PTR]; /* current line in file */
	char current_character; /* current character in line */
	char *current_word; /* current word in the line */
	int line_counter = 1; /* counts the lines */
	int result;
	int create_entry_file = 0;
	int create_extern_file = 0;
	int IC = 0; /* instruction counter */
	int DC = 0;
	int i = 0;
	char entry_symbols[MAX_SYMBOL_LENGTH][MAX_SYMBOL_LENGTH];
	char extern_symbols[MAX_SYMBOL_LENGTH][MAX_SYMBOL_LENGTH];
	int listOfData[MAX_LENGTH_LINE];
	int listOfInstruction[MAX_LENGTH_LINE];
	int binaryWord = 0;

	/* Iterating over the file till the end is reached */
	while (end_of_file(file_ptr) == 0 && 1) { /* skips whitespaces and checks the file */

	/* Fetch the current character in the line */
	current_character = fgetc(file_ptr); /* gets the current character in line */
		/* Check if the line is a comment */
		if (current_character == ';') { /* checks if the line is a comment line */
			fgets(current_line, MAX_LENGTH_LINE_PTR, file_ptr);
			line_counter++;
		}
		/* Check if it's the end of the line */
		if (current_character == '\n') { /* checks if reached to next line */
			line_counter++;
		}

		fseek(file_ptr,-1,SEEK_CUR);
		fgets(current_line, MAX_LENGTH_LINE_PTR, file_ptr);

		current_word = get_word(file_ptr);
		/* Evaluate the current word to determine the corresponding action */
		switch (check_word(current_word, line_counter, 0)) {
			case 1:
				result = 1;
				break;
			case 2:
				result = 2;
				break;
			case 3:
				result = 3;
				break;
			default:
				break;
		}

		if (result == 1 || result == 2) {

			skip_whitespaces(file_ptr);

			if (strcmp(current_word, ".entry") == 0) {
				current_word = get_word(file_ptr);
				while (i < strlen(entry_symbols[i])) {
					if (strcmp(current_word, entry_symbols[i]) == 0) {
						create_entry_file = 1;
						break;
					}

					if (create_entry_file != 1) {
						handle_error(ERROR_UNDECLEARED_ENTRY, line_counter);
					}
				}
			}

			else if (strcmp(current_word, "current_word") == 0) {
				current_word = get_word(file_ptr);

					while (i < strlen(extern_symbols[i])) {
						if (strcmp(current_word, extern_symbols[i]) == 0) {
							create_extern_file = 1;
							break;
						}
					}

					if (create_extern_file != 1) {
						handle_error(ERROR_UNDECLEARED_EXTERN, line_counter);
					}
			}


			else if (strcmp(current_word, ".mov") == 0) {

			}

			else if (strcmp(current_word, ".cmp") == 0) {

			}
		}
    	}
	/* Process and print the final binary words */
	print_64(listOfData, listOfInstruction, binaryWord, IC, DC);
	rewind (file_ptr);
	free(current_word);
	free(file_ptr);
	return 1;
}
