#ifndef BINARY_H
#define BINARY_H

#include <stdint.h>
#include "globals.h"
#include "utils.h"

/*Convert a given assembly instruction to its binary representation.
 *
 * @param machineCode       The output string to store the machine code.
 * @param keyWord           The assembly instruction keyword.
 * @param listOfInstruction Not used in this function (maybe a legacy or external reference).
 * @param numberLine        The line number of the assembly instruction (for error reporting).
 */
void line_instruction_binary(char *machineCode, char *keyWord, int listOfInstruction, int numberLine);

/**
 * Identify the type of operand (e.g., register, label, string, number).
 *
 * @param operand The operand string to analyze.
 * @return The type of the operand.
 */
OperandType identify_operand_type(char *operand);

/**
 * Generate the binary representation for an instruction of group 1 (two operands).
 *
 * @param operands       The operands for the instruction.
 * @param currOpcode     The opcode of the instruction.
 * @param typeOfKeyWord  The type of instruction (group 1).
 * @param numberLine     Line number for potential error messages.
 * @return The instruction in binary form.
 */
instruction_word create_instruction_1(char *operands, int currOpcode, int typeOfKeyWord, int numberLine);

/**
 * Generate the binary representation for an instruction of group 2 (single operand).
 *
 * @param operands       The operand for the instruction.
 * @param currOpcode     The opcode of the instruction.
 * @param typeOfKeyWord  The type of instruction (group 2).
 * @param numberLine     Line number for potential error messages.
 * @return The instruction in binary form.
 */
instruction_word create_instruction_2(char *operands, int currOpcode, int typeOfKeyWord, int numberLine);

/**
 * Generate the binary representation for an instruction of group 3 (no operands).
 *
 * @param operands       Ignored for group 3.
 * @param currOpcode     The opcode of the instruction.
 * @param typeOfKeyWord  The type of instruction (group 3).
 * @param numberLine     Line number for potential error messages.
 * @return The instruction in binary form.
 */
instruction_word create_instruction_3(char *operands, int currOpcode, int typeOfKeyWord, int numberLine) ;

/**
 * Convert data or string guidance to binary representation.
 *
 * @param machineCode    The output string to store the machine code.
 * @param keyWord        The keyword (either data or string).
 * @param dataList       The data to be converted.
 * @param typeOfGuidence The type of guidance (either data or string).
 * @param numberLine     The line number of the assembly instruction (for error reporting).
 */
void line_guidence_binary(char *machineCode, char *keyWord, long dataList, int typeOfGuidence, int numberLine);

/**
 * Convert an integer to its binary string representation.
 *
 * @param machineCode The output string to store the machine code.
 * @param curr        The integer value to convert.
 * @param numOfBit    The number of bits in the output.
 */
void int_to_binary(char *machineCode, int curr, int numOfBit);

/**
 * Encode a given input string into base64 format.
 *
 * @param input         The input data to encode.
 * @param inputLength   The length of the input data.
 * @return The base64 encoded string.
 */
char *base64_encode(unsigned char *input, size_t inputLength);

/**
 * Encode a 12-bit binary word to its base64 representation.
 *
 * @param binaryWord The 12-bit binary word to encode.
 * @return The base64 encoded string.
 */
char *encode_binary_word_to_base64(uint16_t binaryWord);

#endif
