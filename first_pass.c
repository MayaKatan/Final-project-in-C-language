#include <stdio.h>
#include <stdlib.h>
#include "first_pass.h"
#include "table.h"
#include "utils.h"
#include "globals.h"
#include "binary.h"

/* 
This function processes the first pass of the given assembly file. 
It parses the lines and creates the necessary tables and counters.
Parameters:
- asFile: Pointer to the source assembly file.
- currLine: Current line being processed.
- symbolTable: The table holding symbols.
- IC: Instruction counter.
- DC: Data counter.
- lineNumber: Line number being processed.
Return value:
- Returns 1 for success.
*/
int first_pass(FILE *asFile, char *currLine, SymbolTable *symbolTable, long *IC, long *DC, int lineNumber) {

	int currNumberLine = 0; /*count the number of lines*/
	int guidenceLineCount = 0;
	char tmpCurrLine[MAX_LENGTH_LINE], label[MAX_SYMBOL_LENGTH], keyWord[MAX_LENGTH_KEY_WORD], operands[MAX_LENGTH_LINE];
	int typeOfSentence = type_of_sentence(currLine); /*check the type of line*/
	data_image_ptr instructionHead = NULL, instructionTail = NULL, ptrInstruction;
	data_image_ptr guidenceHead = NULL, guidenceTail = NULL;
	Symbol *symbolHead = NULL, *symbolTail = NULL, *ptrSymbol;

	IC = 0; /*instruction counter*/
	DC = 0; /*data counter*/

	currNumberLine++;

	if (typeOfSentence == INSTRUCTION_LINE || typeOfSentence == GUIDANCE_LINE) {
		strcpy(tmpCurrLine, currLine);
		get_data_from_line(tmpCurrLine, label, keyWord, operands, lineNumber);

		/*checking if the label exist, if not it will add to the symbol table*/
		if (label[0] != '\0') {
			int checkLabel = symbol_exists(&label[0], symbolTable);
			if (checkLabel) {
				handle_error(LABEL_EXIST, lineNumber);
			}
		}
		/*create the data image*/
		if (typeOfSentence == INSTRUCTION_LINE ) {
			if (instructionHead == NULL) {
				IC = IC + line_data_image(&instructionHead, NULL, currLine, *IC, typeOfSentence, keyWord, operands, currNumberLine, symbolTable, IC, DC);
				instructionHead->next = NULL;
				instructionTail = instructionHead;
			}
			else {
				IC = IC + line_data_image(&ptrInstruction, NULL, currLine, *IC, typeOfSentence, keyWord, operands, currNumberLine, symbolTable, IC, DC);
				instructionTail->next = ptrInstruction;			
				instructionTail = instructionTail->next;
			}
		}
		else {
			if (guidenceHead == NULL) {
				guidenceLineCount = line_data_image(&guidenceHead, &guidenceTail, currLine, *IC, typeOfSentence, keyWord, operands, 	currNumberLine, symbolTable, IC, DC);
			}
			else {
				guidenceLineCount = line_data_image(&guidenceHead, &guidenceTail, currLine, *IC, typeOfSentence, keyWord, operands, currNumberLine, symbolTable, IC, DC);
			}
			DC = DC + guidenceLineCount;
		}

		/*ceate symbol table*/
		if ((label[0] != '\0')|| (!strcmp(keyWord, ".extern"))) {
			long currAddress;
			
			strcat(label, "\0");
			if (typeOfSentence == INSTRUCTION_LINE) { /*the line is instruction*/
				currAddress = *IC - NEXT_ADDRESS;
			}
			else { /*the other lines*/
				currAddress = *IC + *DC - guidenceLineCount;
			}
			if (symbolHead == NULL) {
				if (!strcmp(keyWord, ".extern")) {
					symbolHead = insert_symbol(operands, 0, EXTERN_SYMBOL); /*the value of extern is 0*/
				}
				else { /*the other symbols like entry*/
					symbolHead = insert_symbol(label, currAddress, typeOfSentence);
				}
				symbolHead->next = NULL;
				symbolTail = symbolHead;
			}

			else {
				if (!strcmp(keyWord, ".extern")) {
					ptrSymbol = insert_symbol(operands, 0, EXTERN_SYMBOL);
				}
				else {
					ptrSymbol = insert_symbol(label, currAddress, EXTERN_SYMBOL);
				}
				symbolTail->next = ptrSymbol;
				symbolTail = symbolTail->next;
			}
		}
	} /*end of if condition typeOfSentence == INSTRUCTION_LINE || typeOfSentence == GUIDANCE_LINE*/
	
	free(currLine);

	
	return 1;
}/*end of void first_pass(FILE *source_file)*/ 



/* 
This function processes a line to create a data image. 
Parameters:
- currLinePtr: Current line being processed.
- tailPtr: End of the data image list.
- currLine: The current line text.
- address: Current address.
- typeOfSentence: Type of line (instruction or guidance).
- keyWord: The keyword of the line.
- operands: The operands of the line.
- numberLine: Line number being processed.
- symbolTable: Symbol table.
- IC: Instruction counter.
- DC: Data counter.
Return value:
- Returns the number of data/image lines created.
*/
int line_data_image(data_image_ptr *currLinePtr, data_image_ptr *tailPtr, char *currLine, long address, int typeOfSentence, char *keyWord, char *operands, int numberLine, SymbolTable *symbolTable, long *IC, long *DC) {

	int srcCodeflag = 1, strLength = strlen(currLine), countLine = 0, dataIndex = 0, i, listOfData[MAX_LENGTH_LINE], listOfInstruction[MAX_LENGTH_LINE], numOfOperands;
	long currAddress = address;
	char curr_char;
	int *listOfOperands = (int*)malloc(MAX_LENGTH_LINE * sizeof(int));
	data_image_ptr currPtr = (data_image*) malloc(sizeof(data_image));

	if (!currPtr) {
		handle_error(ERROR_MEMORY_ALLOCATION, numberLine);
	}

	switch (typeOfSentence)
	{
		case INSTRUCTION_LINE:
		{
			/*define the correct value*/
			line_instruction_binary(currPtr->machine_code, keyWord, listOfInstruction[dataIndex], numberLine);
			strcpy(currPtr->src_code, currLine);
			currPtr->address = address;
			currPtr->next = NULL;
			(*IC)++;

			/*The pointer moves to the next position after it*/
			(*currLinePtr) = currPtr;
			return NEXT_ADDRESS;
			break;
		} /*end of instruction_line case*/
		case GUIDANCE_LINE:
		{	
			if (get_type_guidence(keyWord) == 1) { /*if it's ".data"*/
				if (!listOfOperands) {
					printf("Error: Memory allocation failed.\n");
				}
				numOfOperands = operand_list_to_ascii(operands, listOfOperands, numberLine);

				for (i = 0; i < numOfOperands; i++) {
					currPtr = (data_image*)malloc(sizeof(data_image));
					if (!currPtr) {
						printf("Error: Memory allocation failed.\n");
					}
				
					/*convert operand to machine code and update data image*/
					line_guidence_binary(currPtr->machine_code, ".data", listOfData[dataIndex], DATA, numberLine);
			
					/*update source code and data image properties*/
					if (srcCodeflag == 1) {
						strcpy(currPtr->src_code, currLine);
						srcCodeflag = 0;
					}
					currPtr->address = countLine;
					currPtr->next = NULL;

					/*Update linked list of data images*/
					if (*currLinePtr == NULL) {
						*currLinePtr = currPtr;
						(*currLinePtr)->next = NULL;
						*tailPtr = currPtr;
					} 
					else {
						(*tailPtr)->next = currPtr;
						currPtr->next = NULL;
						*tailPtr = currPtr;
					}
				(*DC)++;
				countLine++;
				} /*end for loop*/
				return countLine;
			} /*end of if .data*/

			else if(get_type_guidence(keyWord) == 2) { /*if it's ".string"*/
				for (i = 0; i<strLength; i++) {
					data_image_ptr tempLine = (data_image_ptr)malloc(sizeof(data_image));	
					if (!tempLine) {
						handle_error(ERROR_MEMORY_ALLOCATION, numberLine);
					}
		
					curr_char = (i < strLength) ? currLine[i] : '\0'; /*add the null terminator and update DC*/

					/*encode the character and add to the machine code*/
					line_guidence_binary(tempLine->machine_code, ".string", listOfData[dataIndex], STRING, numberLine);

					/*update pointers and add source code and address*/
					strcpy(tempLine->src_code, currLine);
					tempLine->address = currAddress;
	
					if ((*currLinePtr) == NULL) {
						(*currLinePtr) = tempLine;
						(*currLinePtr)->next = NULL;
						(*tailPtr) = tempLine;
					} 
					else {
						(*tailPtr)->next = tempLine;
						tempLine->next = NULL;
						(*tailPtr) = tempLine;
					}
					(*DC)++;
				} /*end of for loop*/
			} /*end of else if*/	
		} /*end of guidance line case*/
	} /*end of switch*/

	free(listOfOperands);
	free(currPtr);
	return 0;
} /*end of line_data_image function*/



