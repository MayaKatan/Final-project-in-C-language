#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define MAX_LENGTH_LINE 81
#define MAX_OPCODES 16
#define MAX_LENGTH_KEY_WORD 10
#define MACHINE_CODE_LENGTH 12
#define MAX_MEMORY_SIZE 1024
#define MAX_LABEL_LENGTH 31
#define MAX_SYMBOL_LENGTH 31
#define NEXT_ADDRESS 1
#define SYMBOL_TYPE_DATA 'd'
#define MAX_LENGTH_LINE_PTR 82


/* Enum for error codes */
typedef enum {
	ERROR_FILE_READ = 1,            
	ERROR_EMPTY_FILE ,           
	ERROR_EOF_REACHED,           
	ERROR_LINE_LENGTH, 
	ERROR_WORD_LENGTH, 
	ERROR_SYMBOL_LENGTH, 
	ERROR_INVALID_WORD, 
	ERROR_INVALID_SYMBOL, 
	ERROR_INVALID_OPERATION,     
	ERROR_WRONG_OPERAND_COUNT,   
	ERROR_INVALID_OPERAND_TYPE,  
	ERROR_REGISTER_NOT_FOUND,       
	ERROR_TOO_MANY_OPERANDS,  
	ERROR_MEMORY_ALLOCATION,
	LABEL_EXIST,
	ERROR_UNDECLEARED_ENTRY,
	ERROR_UNDECLEARED_EXTERN,
	ERROR_MULTIPLE_SYMBOLS
} ErrorCode;



/* Structure defining a machine word */

typedef struct { 
    unsigned int data: 12;
} data_word;


/* Enumeration for addressing methods of operands */
typedef enum symbol_type{
	DATA_SYMBOL = 0,
	STRING_SYMBOL,
	ENTRY_SYMBOL,
	EXTERN_SYMBOL 
} symbol_type;

/* Forward declaration of data_image structure */
typedef struct data_image *data_image_ptr;

/* Structure for data image */
typedef struct data_image{
	long address; /*where the line should be stored in memory*/
	char operand_count; /*number of operands*/
	char operands[MAX_MEMORY_SIZE][MACHINE_CODE_LENGTH];
	char src_code[MAX_LENGTH_LINE + 2];
	char machine_code[MAX_LENGTH_LINE + 2];
	data_image_ptr next;
}data_image;

/* Enumeration for instruction group types */
typedef enum {
	INSTRUCTION_GROUP_1 = 1,
	INSTRUCTION_GROUP_2,	
	INSTRUCTION_GROUP_3
}instruction_type;

/* Enumeration for size of a byte */
enum size_byte{
	WORD = 12
};

/* Enumeration for operand types */
typedef enum {
    OPERAND_REGISTER,
    OPERAND_LABEL,
    OPERAND_STRING,
    OPERAND_NUMBER
} OperandType;

/* Structure for Operand */
typedef struct {
    char name[MAX_OPCODES];
    OperandType type;
} Operand;

/* Enumeration for types of guidance */
typedef enum type_of_guidance{ 
	DATA = 1,
	STRING,
	ENTRY,
	EXTERN
}type_guide;

/* Structure for instruction line */
struct instruction_line {
	unsigned int ARE: 2;
	unsigned int operandDst: 3;
	unsigned int opcode: 4;
	unsigned int operandSrc: 3;
	char *name;
}instruction_line;

/* Union for instruction word */
typedef union instruction_word {
    struct instruction_line instruction_line;
    int size;
} instruction_word;

/* Union for guidance data */
typedef union guidanceData{
	struct guidance_word{
		unsigned int word: 12;	
	}guidance;
	int size;
}DataCode;

/* Defines a machine word of registers operands - Can contain destination register & source register */
typedef struct register_word {
    unsigned int ARE: 2;
    unsigned int dest_register: 5;
    unsigned int src_register: 5;
} register_word;

/* Structure for entry symbols */
typedef struct entry_symbols {
    char name[MAX_SYMBOL_LENGTH];
    int dec_num;
    struct entry_symbols *next_entry;
} entry_symbols;

/* Structure for extern symbols */
typedef struct extern_symbols {
    char name[MAX_SYMBOL_LENGTH];
    int dec_num;
    struct extern_symbols *next_extern;
} extern_symbols;


#endif 
