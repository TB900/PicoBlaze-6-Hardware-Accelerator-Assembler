#ifndef parser_h
#define parser_h

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Instruction enumerables
typedef enum instruction {
	ADD,
	STORE,
	FETCH,
	SL0
} instruction;

// Linked list structure
typedef struct ins_node {
	instruction ins;
	char *op1;
	char *op2;
	int state;
	struct ins_node *next;
} dataflow;

// User accessible parsing functions
void print_dataflow(dataflow **input);
int parse_assembly(char *assembly_file, dataflow **assembly);

#endif