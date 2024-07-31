#include "globals.h"
#include "utils.h"
#include "globals.h"
#include "binary.h"
#include "table.h"

/**
 * Skips whitespaces in the current line of the file.
 * @param file_ptr Pointer to the source file.
 */                                                    
void skip_whitespaces (FILE *file_ptr) {

	char current_character = fgetc(file_ptr); /* current character in the line */

	while (current_character != EOF && current_character != '\n' && (current_character == ' ' || current_character == '\t')) {
		fseek(file_ptr ,-1 ,SEEK_CUR);
		current_character = fgetc(file_ptr);
	}
}

/**
 * Checks the length of lines and words in the current line of the file.
 * @param file_ptr Pointer to the source file.
 * @param line_counter Current line number.
 * @return Returns the character count of the line.
 */    
int check_line (FILE *file_ptr, int line_counter) {

	char current_character = fgetc(file_ptr); /* current character in the line */
	int character_counter = 0;
	int word_length_counter = 0;
	int big_word = 0;

	while (current_character != EOF && current_character != '\0' && current_character != '\n') {

		if (current_character != ' ' && current_character != '\t') {
			word_length_counter++;
			character_counter++;
			current_character = fgetc(file_ptr); /* current character in the line */
		}

		if (word_length_counter > MAX_LENGTH_LINE) {
			big_word = 1;
		}

		else {
			word_length_counter = 0;			
			character_counter++;
			current_character = fgetc(file_ptr); /* current character in the line */
	}	}
	
	if (big_word == 1) {
		handle_error(ERROR_WORD_LENGTH, line_counter);
	}

	else if (character_counter > MAX_LENGTH_LINE) {
		handle_error(ERROR_LINE_LENGTH, line_counter);
	}

	return character_counter;
}

/**
 * Verifies if the given word is a valid directive, opcode, or symbol.
 * @param word The word to check.
 * @param line_counter Current line number.
 * @param enable_error If set to 1, will trigger an error handler for invalid words.
 * @return An integer signifying the type of word or 0 for none.
 */
int check_word (char *word, int line_counter, int enable_error) {
	
	int i=0;
	char *directive[MAX_LENGTH_LINE];
	char opcodeTable[MAX_OPCODES];
	char symbols[MAX_SYMBOL_LENGTH];

	for (i = 0; i <= 15; i++) {
	
		if (i <= 3) {

			if (strcmp(word, directive[i]) == 0) {
				return 1;
			}
		}

		if (strcmp(word, opcodeTable[i]) == 0) {
			return 2;
		}

		if (strcmp(word, symbols[i]) == 0) {
			return 3;
		}
	}

	if (enable_error == 1) {
		handle_error(ERROR_INVALID_WORD, line_counter);
	}
	
	return 0;
}


/**
 * Determines if the current line in the file is empty.
 * @param file_ptr Pointer to the source file.
 * @return Returns 1 if the line is empty, otherwise 0.
 */
int empty_line (FILE *file_ptr) {

	char current_character = fgetc(file_ptr); /* current character in the line */
	skip_whitespaces(file_ptr);

	if (current_character == '\n') {
		return 1;
	}

	return 0;
}

/**
 * Determines if the file pointer has reached the end of the file.
 * @param file_ptr Pointer to the source file.
 * @return Returns 1 if at the end of the file, otherwise 0.
 */
int end_of_file (FILE *file_ptr) {

	char current_character = fgetc(file_ptr); /* current character in the line */

	skip_whitespaces(file_ptr);

	if (current_character == EOF) {
		return 1;
	}

	return 0;
}

/**
 * Extracts and returns the next word from the file.
 * @param file_ptr Pointer to the source file.
 * @return A dynamically allocated string containing the word. Caller must free.
 */
char *get_word (FILE *file_ptr) {
	
	char current_character = fgetc(file_ptr); /* current character in the line */
	char *word = (char*)malloc(MAX_LENGTH_LINE * sizeof(char));
	int i = 0;

	while (current_character != EOF && current_character != '\0' && current_character != '\t' && current_character != ' ' && current_character != '\n') {

		word[i] = current_character;
        	i++;
		fseek(file_ptr ,-1 ,SEEK_CUR);
		current_character = fgetc(file_ptr);
	}

	word[i] = '\0';
	free(word);
	return word;
}

/**
 * Converts and prints the given data and instruction lists into base 64 format.
 * @param listOfData Data list to process.
 * @param listOfInstruction Instruction list to process.
 * @param binaryWord Binary word representation.
 * @param IC Instruction counter.
 * @param DC Data counter.
 */
void print_64 (int listOfData[MAX_LENGTH_LINE], int listOfInstruction[MAX_LENGTH_LINE], uint16_t binaryWord, int IC, int DC) {

	int i = 0;
	FILE *file_ptr;

	if (IC == 0 && DC == 0) {
		printf ("Error in creating output 64 file: failed to open a file for 64 base\n");
		return;
	}

	while (i <= IC) {
		if (listOfInstruction[i] != 3) {
			printf("%s ", encode_binary_word_to_base64(binaryWord));
		}
	}

	i = 0;

	while (i <= DC) {
		if (listOfInstruction[i] == 3) {
			printf("%s ", encode_binary_word_to_base64(binaryWord));
		}
	}

	fprintf (file_ptr, "%d ", IC);
	fprintf (file_ptr,"%d", DC);
}
