#ifndef MAIN_H
#define MAIN_H

#include "preassembler.h"
#include "table.h"
#include "globals.h"
#include "utils.h"

/**
 * Transforms the provided assembly file into machine code.
 * Invokes functionalities from other included headers to achieve the transformation.
 * @param fileName The name/path of the assembly file to process.
 * @return Success status of assembly to machine code conversion.
 */
int assembly_to_machine_code(char* fileName);

#endif
