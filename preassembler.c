#include <stdlib.h>
#include <stdio.h>
#include "table.h"
#include "utils.h"
#include <string.h>
#include "globals.h"

#define MAX_MACRO_SIZE_NAME 40

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
int preassembler_file(char* fileName) {
    /** Pointer to the intermediate ".am" file after pre-assembly. */
    FILE *amFile; 
    /** Name of the intermediate ".am" file. */
    char *amFileName = filename_suffix(fileName, ".am");
    /** Name of the original ".as" assembly file. */
    char *asFileName = filename_suffix(fileName, ".as");
    /** Pointer to the original ".as" assembly file. */
    FILE *asFile = fopen(asFileName, "r"); 

    char currLine[MAX_LENGTH_LINE + 2]; /* Holds the current line being read. */
    char *macroName = NULL;             /* Stores the name of a discovered macro. */
    char *macroContent = NULL;          /* Stores the content/body of a discovered macro. */	
    /** Table to store and manage discovered macros. */
    macro_table *macroTable = create_macro_table();
    /** Flag to determine if currently reading inside a macro definition. */
    int macroFlag = 0; 
    macro *currentMacro;
    macro *temp;                        /* Temporary macro pointer for freeing memory. */
    char *expendLine;                   /* Expanded line after macro substitution. */

    /* Check if the original ".as" file opened successfully. */
    if(asFile == NULL) {
        printf("The file is failed.\n");
        free(amFileName);
        free(asFileName);
        free_macro_table(macroTable);
        return 0;
    }

    /* Check if the new ".am" file was created successfully. */
    amFile = fopen(amFileName, "w+");
    if(amFile == NULL) {
        printf("The file is failed.\n");
        free(amFileName);
        free(asFileName);
        fclose(asFile);
        free_macro_table(macroTable);
        return 0;		
    }

    /* Iterate through the ".as" file line by line. */
    while(fgets(currLine, MAX_LENGTH_LINE + 2, asFile) != NULL) {
        printf("current line: %s\n", currLine);
        /* Check for the start of a macro definition. */
        if (strstr(currLine, "mcro") != NULL) {
            macroFlag = 1;
            macroName = (char *)malloc(MAX_MACRO_SIZE_NAME);
            strcpy(currLine, currLine + 4);
            skip_white_spaces(currLine);
            strcpy(macroName, currLine);

            macroContent = (char *)malloc(MAX_LENGTH_LINE + 2);

            /* Continue reading lines until the end of the macro definition is found. */
            while (fgets(currLine, MAX_LENGTH_LINE + 2, asFile) != NULL) {
                if (strstr(currLine, "endmcro") != NULL) {
                    macroFlag = 0;
                    add_macro_to_table(macroTable, macroName, macroContent);
                    break;
                }
                if (macroFlag) { /* Append the line to the macro content if inside a macro definition. */
                    strcat(macroContent, currLine);
                }
            }
        } else { 
            /* If the current line contains a macro name, replace it with its content. */
            expendLine = expend_macro(currLine, macroTable);
            if (expendLine != NULL) {
                fputs(expendLine, amFile);
                free(expendLine);
            } else { /* Write the regular line to the ".am" file. */
                fputs(currLine, amFile);
            }
        }
    }

    /* Free the allocated memory for discovered macros. */
    currentMacro = macroTable->head;
    while (currentMacro != NULL) {
        temp = currentMacro;
        currentMacro = currentMacro->next;
        free(temp->macroName);
        free(temp->macroContent);
        free(temp);
    }

    /* Cleanup resources. */
    free(macroTable);
    free(macroName);
    free(macroContent);
    fclose(asFile);
    fclose(amFile);

    return 1;
}
