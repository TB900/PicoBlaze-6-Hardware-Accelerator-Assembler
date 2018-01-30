#ifndef parser_h
#define parser_h

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum instruction {
	ADD,
	STORE,
	FETCH,
	SL0
} instruction;

typedef struct ins_node {
	instruction ins;
	char *op1;
	char *op2;
	int state;
	struct ins_node *next;
} dataflow;

int parse_assembly(char *assembly_file, dataflow **assembly);

#endif