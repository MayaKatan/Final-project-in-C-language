#include <stdlib.h>
#include <stdio.h>
#include "table.h"
#include "utils.h"
#include "globals.h"

/**
 * @brief Pre-assembles the provided assembly file.
 * 
 * This function reads an assembly file and performs macro substitution to produce an intermediate ".am" file.
 * The function reads each line of the ".as" file and looks for macro definitions. If a macro is found, 
 * it gets stored in a macro table. Once all macros have been identified, the function will then substitute 
 * instances of these macros in the assembly file with their content.
 * 
 * @param fileName The name of the assembly file to be pre-assembled (without the ".as" extension).
 * @return 1 if the pre-assembly process completes successfully; 0 otherwise.
 */

int preassembler_file(char* fileName);
