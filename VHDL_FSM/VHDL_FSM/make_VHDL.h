#ifndef make_VHDL_h
#define make_VHDL_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "instruction_templates.h"

int make_VHDL(FILE *VHDL, char *entity, int num_states, int pb_sleep, int num_inputs, int num_outputs, instruction last_ins, dataflow **assembly);
void make_Testbench(FILE *TB, char *entity, int num_states, int num_inputs, int num_outputs, int num_bits);

#endif