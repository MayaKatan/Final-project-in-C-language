#ifndef _TABLE_H
#define _TABLE_H
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

#define TABLE_SIZE 80

/**
 * @brief Structure for defining a macro.
 * 
 * Contains the name and content of a macro, as well as a pointer to the next macro in a linked list.
 */
typedef struct macro {
	char *macroName;
	char *macroContent;
	struct macro *next;
} macro;

/**
 * @brief Structure for a macro table.
 * 
 * Contains a head pointer that points to the first macro in the table (linked list).
 */
typedef struct {
	macro *head;
} macro_table;

/* Define an entry in a symbols table */
typedef struct s_table_entry{
	char *symbol;
	int symbol_addr;
	symbol_type symbol_type;
	struct s_table_entry *next;
} s_table_entry;

/**
 * @brief Structure for an entry in the symbols table.
 * 
 * Contains the name, address, type of the symbol, and a pointer to the next symbol entry.
 */
typedef struct {
	char name[MAX_SYMBOL_LENGTH];
	int value;
	char type;
	struct Symbol* next;
} Symbol;

/**
 * @brief Structure for a symbols table.
 * 
 * Contains an array of symbols, a double pointer for entries, and a count of the symbols.
 */
typedef struct SymbolTable{
	Symbol symbols[MAX_SYMBOL_LENGTH];
	char **entries;
	int count;
} SymbolTable;

/** Global symbol table instance. */
SymbolTable symbol_table; 

/** Function prototypes. */
macro_table *create_macro_table();

void add_macro_to_table(macro_table *table, char *name, char *content);

char *expend_macro(char *line, macro_table *table);

void free_macro_table(macro_table *table);

SymbolTable *create_symbol_table();

#endif
