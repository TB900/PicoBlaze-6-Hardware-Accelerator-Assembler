#include <stdio.h>
#include <stdlib.h>
#include "make_VHDL.h"
#include "parser.h"

int main(int argc, char *argv[]) {

	if (argc != 2) {
		argv[1] = "assembly.psm";
	}

	dataflow *assembly = NULL;
	assembly = (dataflow *)malloc(sizeof(dataflow));
	assembly->next = NULL;

	parse_assembly(argv[1], &assembly);

	print_dataflow(&assembly);
	
	/*
	int num_states;
	int num_inputs;
	int num_outputs;
	sscanf_s(argv[1], "%d", &num_states);
	sscanf_s(argv[2], "%d", &num_inputs);
	sscanf_s(argv[3], "%d", &num_outputs);

	FILE* VHDL;
	FILE* TB;

	fopen_s(&VHDL, "FSM.vhdl", "w");
	fopen_s(&TB, "FSM_tb.vhdl", "w");

	make_VHDL(VHDL, num_states, num_inputs, num_outputs);
	make_Testbench(TB, num_states, num_inputs, num_outputs);

	fclose(VHDL);
	fclose(TB);
	*/
	return 0;
}