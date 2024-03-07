#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include <stdio.h>
#include <stdlib.h>
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
int first_pass(FILE *asFile, char *currLine, SymbolTable *symbolTable, long *IC, long *DC, int lineNumber);

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
int line_data_image(data_image_ptr *currLinePtr, data_image_ptr *tailPtr, char *currLine, long address, int typeOfSentence, char *keyWord, char *operands, int numberLine, SymbolTable *symbolTable, long *IC, long *DC);


#endif
