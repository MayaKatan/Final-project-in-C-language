#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"



/*hold opcode information*/
struct opcodeInfo {
	char *name;
	int code;
}opcodeInfo; 

/*table of the opcodes and their hexadecimal codes*/
struct opcodeInfo opcodeTable1[MAX_OPCODES] = {
	{"mov", 0},
	{"cmp", 1},
	{"add", 2},
	{"sub", 3},
	{"lea", 6}
};
struct opcodeInfo opcodeTable2[MAX_OPCODES] = {
	{"not", 4},
	{"clr", 5},
	{"inc", 7},
	{"dec", 8},
	{"jmp", 9},
	{"bne", 10},
	{"red", 11},
	{"prn", 12},
	{"jsr", 13}
};
struct opcodeInfo opcodeTable3[MAX_OPCODES] = {
	{"rts", 14},
	{"stop", 15}
};


/*this function add the suffix .am to the original file name*/
char *filename_suffix(char *str1, char *str2)
{
	char *str = (char*)malloc(strlen(str1) + strlen(str2) + 1);
	strcpy(str, str1);
	strcat(str, str2);
	return str;
}

/*this function skip all the white spaces in the line*/
void skip_white_spaces(char currLine[]) {
    int len = strlen(currLine);
    int src = 0, dest = 0; /*use to track the beginning and end of the line*/

    while (src < len) { /*check all the chars in line*/
        if (!isspace(currLine[src])) {
            currLine[dest] = currLine[src];
            dest++;
        }
        src++;
    }

    currLine[dest] = '\0'; /*it is the end of the line*/
}


/*this function classified the type of the sentence - note, empty, guidence or instruction line*/
int type_of_sentence(char *currLine) {
	
	skip_white_spaces(currLine);
	while (*currLine != '\0') {
		if ((strchr(currLine, '\t')) || (strchr(currLine, ' '))) {
			return EMPTY_LINE;
		}	
		else if (strchr(currLine, '.')) {
			return GUIDANCE_LINE;
		}
		else if (strchr(currLine, '\n')) {
			return EMPTY_LINE;
		}
		else if (strchr(currLine, ';')) {
			return NOTE_LINE;
		}
		else {
			return INSTRUCTION_LINE;
		}
	}
	return INSTRUCTION_LINE;
}

/*this function get the data from the line using tokens*/
void get_data_from_line(char *currLine, char *label, char *keyWord, char *operands, int numberLine) {
	int labelFlag = 0;
	char tempCurrLine[MAX_LENGTH_LINE];
	char *token;

	strcpy(tempCurrLine, currLine);
	strcat(tempCurrLine, "\0");
	skip_white_spaces(tempCurrLine);

	token = strtok(tempCurrLine, " \t\n");

	while(token != NULL) {
		/*this is label*/
		if (strstr(token, ":")) {
			labelFlag = 1;
			strcpy(label, token);
			if (strlen(label) > MAX_SYMBOL_LENGTH) {
				handle_error(ERROR_SYMBOL_LENGTH, numberLine);
			}
		}
		/*this is key word*/
		if (strstr(token, ".")) {
			strcpy(keyWord, token);
		}
		/*this is operand*/
		if (labelFlag == 1) {
			strcpy(operands, token);
		}
		
		token = strtok(NULL, " \t\n"); /*get the next token*/
	}
}


/*The following function checks if the symbol already exists in the table.*/
int symbol_exists(char* symbol_name, SymbolTable* current_table) {
    int i;
    for(i = 0; i < current_table->count ; i++){
	if (strcmp(current_table->symbols[i].name, symbol_name) == 0) {
            return 1;  /* true - symbol exists */
        }   
    }
    return 0;  /* false - symbol doesn't exist */
}


/*this function determine the type of instruction based on the provided key_word (opcode name)*/
int get_instruction_type(instruction_word *keyWord) {
	int i=0;
	int sizeOfKeyWordArray1 = 5;
	int sizeOfKeyWordArray2 = 9;
	int sizeOfKeyWordArray3 = 2;

	for (i=0; i<sizeOfKeyWordArray1; i++) {
		if (strcmp(keyWord->instruction_line.name, opcodeTable1[i].name) == 0) {
			keyWord->instruction_line.ARE = 2;
			keyWord->instruction_line.operandDst = 3;
			keyWord->instruction_line.opcode = 4;
			keyWord->instruction_line.operandSrc = 3;
			/*return opcodeTable1[i];*/		
		}
	}
	for (i=0; i<sizeOfKeyWordArray2; i++) {
		if (strcmp(keyWord->instruction_line.name, opcodeTable2[i].name) == 0) {
			keyWord->instruction_line.ARE = 2;
			keyWord->instruction_line.operandDst = 3;
			keyWord->instruction_line.opcode = 4;
			keyWord->instruction_line.operandSrc = 0;
			/*return opcodeTable2[i];*/
		}
	}
	for (i=0; i<sizeOfKeyWordArray3; i++) {
		if (strcmp(keyWord->instruction_line.name, opcodeTable3[i].name) == 0) {
			keyWord->instruction_line.ARE = 2;
			keyWord->instruction_line.operandDst = 0;
			keyWord->instruction_line.opcode = 4;
			keyWord->instruction_line.operandSrc = 0;
			/*return opcodeTable3[i];*/
		}
	}
	return 0;
} /*if the key word didnt found*/


/*this function represent the operands in ascii*/
int operand_list_to_ascii(char *operands, int *list, int numberLine) {
	int i=0;
	int strFlag = 0; /*there is string as operand*/
	int commaFlag = 0;
	int minusFlag = 0;
	int operandCount = 0;
	int tempNum = 0;
	int operndCounter = 0;

	for (i=0; i<strlen(operands); i++) {
		skip_white_spaces(operands);
		if (operands[i] == 34) { /*check if there is string*/
			strFlag = 1;
			i++;
		}
	} /*end for loop*/
	if (strFlag == 1) { /*if it's .string*/
		for (; i<strlen(operands); i++) {
			if (operands[i] == 34) {
				strFlag = 0;
				break;
			}
			list[operandCount++] = operands[i];
			skip_white_spaces(operands);
		}
		if (strFlag == 1) {
			printf("error: syntax\n");
		}
	} /*.data*/
	else if (operands[i] >= '0' && operands[i] <= '9') { /*check if the current character is a digit*/
		for (;i<strlen(operands);) {
			skip_white_spaces(operands);
			if (operands[i] == ',') {
				if (commaFlag == 0) { /*if there isn't ,*/
					printf("error: syntax\n");
				}
				commaFlag = 0;
				i++;
			}
			else if (commaFlag == 0) {
				while ((operands[i]== '-') || ((operands[i] >= '0') &&( operands[i] <= '9'))) {
					if (operands[i] == '-') {
						minusFlag = 1; /*negative number*/
					}
					else { /*two digit number*/
						tempNum = (tempNum * 10) + (operands[i] = '0'); /*to get the ascii value*/
						i++;
					}
				}
				commaFlag = 1;
				if (minusFlag == 1) { /*update the negative num*/
					tempNum = (-1) * tempNum;
				}
				list[operandCount] = tempNum; /*add the number to the list*/
				tempNum = 0;
				skip_white_spaces(operands);
			}
		} /*end for loop*/
	}
	else { /*.entry .extern*/
		list[operndCounter] = operands[i];
	}
	return operndCounter;
}

/*this function gets the filed word and decide */
int get_type_guidence(char* keyWord) {
	int i=0;
	int numOfTypes = 4;
	const char *(guidenceType[]) = {".data", ".string", "entry", "extern"};
	
	for (i=0; i<numOfTypes; i++) {
		if (strcmp(keyWord, guidenceType[i]) == 0) {
			return i+1; /*the int that match the word type*/
		}
	}
	return 0;
}



/*The function takes a symbol name, a numeric value, and a type. It adds the symbol to the global symbol table within the program.
 If the symbol already exists in the table, the program may return an error (if you were to run the method one after the other on the same symbol name).*/
Symbol *insert_symbol(char* symbol_name, int value, char type) {
	int i;
	Symbol *newSymbol;
	for (i = 0; i < symbol_table.count; i++) {
		if (strcmp(symbol_table.symbols[i].name, symbol_name) == 0) {
		printf("Error: Symbol %s already exists!\n", symbol_name);
		return 0;
		}
	}
	newSymbol =  &symbol_table.symbols[symbol_table.count]; /* Pointer to the new symbol */
	strcpy(newSymbol->name, symbol_name);
	newSymbol->value = value;
	newSymbol->type = type;
	symbol_table.count++; /*enlarging the table*/

	return newSymbol;
}

/*this function create the files .externals and .entries*/
void create_files (struct entry_symbols **head_entry, struct extern_symbols **head_extern, int create_entry_file, int create_extern_file) {
	FILE *entry_ptr;
	FILE *extern_ptr;
	struct entry_symbols *current_entry = *head_entry;	
	struct extern_symbols *current_extern = *head_extern;

	if (create_entry_file == 1) { /*check if entry exists*/
		entry_ptr = fopen(".entries", "w");

		if (entry_ptr == NULL) { /*failed to open file for entries*/
			printf("error in creating .entries file: failed to open file for entries\n");
		}
		else {
			while (current_entry != NULL) { /*print the entries*/
				fprintf(entry_ptr, "%s  ", current_entry->name);
				fprintf(entry_ptr, "%d\n", current_entry->dec_num);
				current_entry = current_entry->next_entry;
			}
			fclose(entry_ptr);
		}
	}
	if (create_extern_file == 1) { /*check if extern exists*/
		extern_ptr = fopen(".externals", "w");
		if (entry_ptr == NULL) { /*failed to open file for externals*/
			printf("error in creating .externals file: failed to open file for externals\n");
		}
		else {
			while (current_extern != NULL) { /*print the externals*/
				fprintf(extern_ptr, "%s  ", current_extern->name);
				fprintf(extern_ptr, "%d\n", current_extern->dec_num);
				current_extern = current_extern->next_extern;
			}
			fclose(extern_ptr);
		}
	}
}		

/*this function get a string and reverse it*/		
void reverse_string(char *str) {
    int length = strlen(str), i;
    for (i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

