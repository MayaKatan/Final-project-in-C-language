/** 
 * utils.h - A utility header file providing various utility functions and 
 * enumerations for the assembler's processing and operations.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "table.h"
#include "globals.h"

/** Enumeration for determining the type of a given sentence/line in the assembly source. */
enum typeOfSentence {
	EMPTY_LINE = 0,
	NOTE_LINE,
	INSTRUCTION_LINE,
	GUIDANCE_LINE
};

/** 
 * Appends a suffix to a given filename.
 * @param str1 Original filename.
 * @param str2 Suffix to be appended.
 * @return Pointer to the new filename.
 */
char *filename_suffix(char *str1, char *str2);

/** 
 * Removes white spaces from a given string.
 * @param currLine The string from which white spaces are to be removed.
 */
void skip_white_spaces(char currLine[]);

/** 
 * Determines the type of a given sentence/line.
 * @param currLine The string whose type is to be determined.
 * @return type of the sentence.
 */
int type_of_sentence(char *currLine);

/** 
 * Extracts data from a given line.
 * @param currLine The string to extract data from.
 * @param label The extracted label.
 * @param keyWord The extracted keyword.
 * @param operands The extracted operands.
 * @param numberLine The current line number.
 */
void get_data_from_line(char *currLine, char *label, char *keyWord, char *operands, int numberLine);

/** 
 * Checks if a symbol exists in the symbol table.
 * @param symbol_name Name of the symbol.
 * @param current_table Symbol table to check against.
 * @return 1 if the symbol exists, 0 otherwise.
 */
int symbol_exists(char* symbol_name, SymbolTable* current_table);

/** 
 * Determines the type of instruction based on the keyword.
 * @param keyWord The keyword to check.
 * @return Type of the instruction.
 */
int get_instruction_type(instruction_word *keyWord);

/** 
 * Converts a comma-separated list of operands to an integer array.
 * @param operands Operand string to be converted.
 * @param list Integer array to store the results.
 * @param numberLine The current line number.
 * @return Count of operands.
 */
int operand_list_to_ascii(char *operands, int *list, int numberLine);

/** 
 * Determines the type of guidance based on the keyword.
 * @param keyWord The keyword to check.
 * @return Type of the guidance.
 */
int get_type_guidence(char *keyWord);

/** 
 * Inserts a new symbol into the symbol table.
 * @param symbol_name Name of the symbol.
 * @param value Value of the symbol.
 * @param type Type of the symbol.
 * @return Pointer to the inserted symbol.
 */
Symbol *insert_symbol(char* symbol_name, int value, char type);

/** 
 * Generates output files for the assembler.
 * @param head_entry Head of the entry symbol list.
 * @param head_extern Head of the extern symbol list.
 * @param create_entry_file Flag to determine if entry file needs to be created.
 * @param create_extern_file Flag to determine if extern file needs to be created.
 */
void create_files (struct entry_symbols **head_entry, struct extern_symbols **head_extern, int create_entry_file, int create_extern_file);

/** 
 * Reverses the given string in place.
 * @param str String to be reversed.
 */
void reverse_string(char *str);

/**
 * Handles an error according to the given error code and line counter.
 * @param error_code The specific code identifying the error.
 * @param line_counter The line number in the source code where the error occurred.
 */
void handle_error(int error_code, int line_counter);

/** 
 * Checks the validity of a line in the file.
 * @param file_ptr Pointer to the file being read.
 * @param line_counter Current line number for error reporting.
 * @return Integer status or error code.
 */
void skip_whitespaces(FILE *file_ptr);

/** 
 * Checks the validity of a line in the file.
 * @param file_ptr Pointer to the file being read.
 * @param line_counter Current line number for error reporting.
 * @return Integer status or error code.
 */
int check_line(FILE *file_ptr, int line_counter);

/** 
 * Validates a word in the context of the assembly language.
 * @param word The word to be checked.
 * @param line_counter Current line number for error reporting.
 * @param enable_error Flag to enable or disable error generation.
 * @return Integer status or error code.
 */
int check_word (char *word, int line_counter, int enable_error);

/** 
 * Checks if a given line in the file is empty.
 * @param file_ptr Pointer to the file being read.
 * @return 1 if the line is empty, 0 otherwise.
 */
int empty_line(FILE *file_ptr);

int second_pass(FILE *file_ptr);

/** 
 * Determines if the end of the file has been reached.
 * @param file_ptr Pointer to the file being read.
 * @return 1 if end of file, 0 otherwise.
 */
int end_of_file(FILE *file_ptr);

/** 
 * Retrieves the next word from the file.
 * @param file_ptr Pointer to the file being read.
 * @return The extracted word.
 */
char *get_word(FILE *file_ptr);

/** 
 * Prints the 64-bit binary data and instructions.
 * @param buffer_data Data buffer containing the binary data.
 * @param buffer_inst Instruction buffer containing the binary instructions.
 * @param binaryWord 64-bit binary word.
 * @param IC Instruction counter.
 * @param DC Data counter.
 */
void print_64 (int listOfData[MAX_LENGTH_LINE], int listOfInstruction[MAX_LENGTH_LINE], uint16_t binaryWord, int IC, int DC);


#endif 
