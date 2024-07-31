#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include "globals.h"
#include "utils.h"
#include "binary.h"


/**
 * Convert a given assembly instruction to its binary representation.
 *
 * @param machineCode       The output string to store the machine code.
 * @param keyWord           The assembly instruction keyword.
 * @param listOfInstruction Not used in this function (maybe a legacy or external reference).
 * @param numberLine        The line number of the assembly instruction (for error reporting).
 */
void line_instruction_binary(char *machineCode, char *keyWord, int listOfInstruction, int numberLine) {
	int currOpcode = 0;
	instruction_word currLine1;
	instruction_word currLine2;
	instruction_word currLine3;

	int typeOfKeyWord = get_instruction_type(keyWord);

	switch (typeOfKeyWord) {
		case INSTRUCTION_GROUP_1:
			currLine1 = create_instruction_1(keyWord, currOpcode, typeOfKeyWord, numberLine);
			int_to_binary(machineCode, currLine1.size, WORD);
			break;

		case INSTRUCTION_GROUP_2:
			currLine2 = create_instruction_2(keyWord, currOpcode, typeOfKeyWord, numberLine);
			int_to_binary(machineCode, currLine2.size, WORD);
			break;
		case INSTRUCTION_GROUP_3:
			currLine3 = create_instruction_3(keyWord, currOpcode, typeOfKeyWord, numberLine);
			int_to_binary(machineCode, currLine3.size, WORD);
			break;
	}
}



/**
 * Identify the type of operand (e.g., register, label, string, number).
 *
 * @param operand The operand string to analyze.
 * @return The type of the operand.
 */
OperandType identify_operand_type(char *operand) {
    if (operand[0] == '@' && operand[1] == 'r' && operand[2] >= '0' && operand[2] <= '7' && operand[3] == '\0') {
        return OPERAND_REGISTER;
    } else if (isdigit(operand[0]) || (operand[0] == '-' && isdigit(operand[1]))) {
        return OPERAND_NUMBER;
    } else if (operand[0] == '"' && operand[strlen(operand) - 1] == '"') {
        return OPERAND_STRING;
    } else {
        return OPERAND_LABEL;
    }
}

/**
 * Generate the binary representation for an instruction of group 1 (two operands).
 *
 * @param operands       The operands for the instruction.
 * @param currOpcode     The opcode of the instruction.
 * @param typeOfKeyWord  The type of instruction (group 1).
 * @param numberLine     Line number for potential error messages.
 * @return The instruction in binary form.
 */
instruction_word create_instruction_1(char *operands, int currOpcode, int typeOfKeyWord, int numberLine) {
	char *operand1 = strtok((char *)operands, ",");
	char *operand2 = strtok(NULL, ",");
	/*idenify the operand types*/
	OperandType operand_1_type = identify_operand_type(operand1);
	OperandType operand_2_type = identify_operand_type(operand2);
	
	instruction_word instr;
	instr.instruction_line.opcode = currOpcode;


	if (!operand1 || !operand2) {
		printf("error: missing operand\n");
		instr.size = 0;
		return instr;
	}

	switch (operand_1_type) {
		case OPERAND_REGISTER:
			instr.instruction_line.operandSrc = operand1[2] - '0'; /*convert int to char*/
			break;
		case OPERAND_LABEL:
			break;
		case OPERAND_STRING:
			instr.instruction_line.operandSrc = *operand1;
			break;
		case OPERAND_NUMBER:
			instr.instruction_line.operandSrc = *operand1;
			break;
	}
	switch (operand_2_type) {
		case OPERAND_REGISTER:
			instr.instruction_line.operandDst = operand2[2] - '0'; /*convert int to char*/
			break;
		case OPERAND_LABEL:
			break;
		case OPERAND_STRING:
			instr.instruction_line.operandDst = *operand2;
			break;
		case OPERAND_NUMBER:
			instr.instruction_line.operandDst = *operand2;
			break;
	}
	instr.instruction_line.ARE = 0;
	return instr;
}	

/**
 * Generate the binary representation for an instruction of group 2 (single operand).
 *
 * @param operands       The operand for the instruction.
 * @param currOpcode     The opcode of the instruction.
 * @param typeOfKeyWord  The type of instruction (group 2).
 * @param numberLine     Line number for potential error messages.
 * @return The instruction in binary form.
 */
instruction_word create_instruction_2(char *operands, int currOpcode, int typeOfKeyWord, int numberLine) {
	OperandType operand_type = identify_operand_type(operands);	
	instruction_word instr;
	instr.instruction_line.opcode = currOpcode;


	if (operands == NULL) {
		handle_error(ERROR_WRONG_OPERAND_COUNT, numberLine);
		return instr;
	}

	switch (operand_type) { /*the source operand is always 0*/
		case OPERAND_REGISTER:
			instr.instruction_line.operandSrc = 0;
			instr.instruction_line.operandDst = operands[2] - '0'; /*convert int to char*/
			break;
		case OPERAND_LABEL:
			break;
		default:
			handle_error(ERROR_INVALID_OPERAND_TYPE, numberLine);
	}	
	instr.instruction_line.ARE = 0;	
	return instr;
}

/**
 * Generate the binary representation for an instruction of group 3 (no operands).
 *
 * @param operands       Ignored for group 3.
 * @param currOpcode     The opcode of the instruction.
 * @param typeOfKeyWord  The type of instruction (group 3).
 * @param numberLine     Line number for potential error messages.
 * @return The instruction in binary form.
 */
instruction_word create_instruction_3(char *operands, int currOpcode, int typeOfKeyWord, int numberLine) {
	instruction_word instr;
	instr.instruction_line.opcode = currOpcode;
	instr.instruction_line.operandSrc = 0; /*the source operand is always 0*/
	instr.instruction_line.operandDst = 0; /*the destination operand is always 0*/
	instr.instruction_line.ARE = 0;

	return instr;
}


/**
 * Convert data or string guidance to binary representation.
 *
 * @param machineCode    The output string to store the machine code.
 * @param keyWord        The keyword (either data or string).
 * @param dataList       The data to be converted.
 * @param typeOfGuidence The type of guidance (either data or string).
 * @param numberLine     The line number of the assembly instruction (for error reporting).
 */
void line_guidence_binary(char *machineCode, char *keyWord, long dataList, int typeOfGuidence, int numberLine) {
	DataCode currData; 
	DataCode currStr;
	char currChar;

	switch(typeOfGuidence) {
		case DATA:
			currData.guidance.word = dataList;
			int_to_binary(machineCode, currData.size, WORD);
			break;
		case STRING:
			currStr.guidance.word = currChar;
			int_to_binary(machineCode, currStr.size, WORD);	
			break;
	} /*end of switch*/	
}




/**
 * Convert an integer to its binary string representation.
 *
 * @param machineCode The output string to store the machine code.
 * @param curr        The integer value to convert.
 * @param numOfBit    The number of bits in the output.
 */
void int_to_binary(char *machineCode, int curr, int numOfBit) {
	unsigned mask = 1;

	for (; mask; mask <<= 1) {
		if (numOfBit != 0) {
			char bit;
			bit = (mask & curr)? '1':'0'; /*Check if the current bit is 1 or 0*/
			strncat(machineCode, &bit, 1); /*Append the bit to the machineCode string*/
			numOfBit--; /*Decrease the number of remaining bytes*/
		}
	}
	strcat(machineCode, "\0"); /*Add the EOF marker to the end of the code*/
	reverse_string(machineCode); /*change the order of bits to match the actual binary representation*/
}

static char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * Encode a given input string into base64 format.
 *
 * @param input         The input data to encode.
 * @param inputLength   The length of the input data.
 * @return The base64 encoded string.
 */
char *base64_encode(unsigned char *input, size_t inputLength) {
	size_t outputLength = 4*((inputLength+2)/3);
	char* encodedData = (char*)malloc(outputLength + 1);
	size_t i, j=0;
	if (encodedData == NULL) {
		return NULL;
	}	
	for (i = 0; i < inputLength; i += 3) {
		uint32_t value = 0;
		int count = 0;
		for (j = 0; j < 3; j++) {
			value <<= 8;
			if (i + j < inputLength) {
				value |= input[i + j];
				count++;
			}
		}

		for (j=0; j<4; j++) {
			encodedData[i*4/3+j] = count > j ? base64_chars[(value >> (6*(3-j))) & 0x3F] : '=';
		}
	}

	encodedData[outputLength] = '\0';
	return encodedData;
	free(encodedData);
}

/**
 * Encode a 12-bit binary word to its base64 representation.
 *
 * @param binaryWord The 12-bit binary word to encode.
 * @return The base64 encoded string.
 */
char *encode_binary_word_to_base64(uint16_t binaryWord) {
	/*convert the 12-bit binary word to 2 bytes*/
	unsigned char bytes[2];
	char *encoded = base64_encode(bytes, 2);
	bytes[0] = (binaryWord >> 4) & 0xFF;
	bytes[1] = ((binaryWord & 0xF) << 4) & 0xF0;

	free(encoded);
	return encoded;
}


