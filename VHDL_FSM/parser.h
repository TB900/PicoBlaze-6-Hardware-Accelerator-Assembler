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
	SL0,
	INPUT,
	OUTPUT,
	SL1,
	SLA,
	SLX,
	SR0,
	SR1,
	SRA,
	SRX,
	RL,
	RR,
	ADDCY,
	SUB,
	SUBCY,
	AND,
	OR,
	XOR,
	LOAD,
	COMPARE,
	COMPARECY,
	TEST,
	TESTCY,
	LOOP,
	END_LOOP
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
void parse_assembly(char *assembly_file, dataflow **assembly, int *num_ST, instruction *last_ins);

#endif