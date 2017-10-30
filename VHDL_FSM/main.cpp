#include <stdio.h>
#include <stdlib.h>
#include "make_VHDL.h"

int main(int argc, char *argv[]) {

	if (argc != 4) {
		fprintf(stderr, "Invalid Argument: Please supply: num of states, num of inputs, num of outputs!");
		return 0;
	}

	int num_states = *argv[1] - '0';
	int num_inputs = *argv[2] - '0';
	int num_outputs = *argv[3] - '0';

	FILE* VHDL;
	FILE* TB;

	fopen_s(&VHDL, "FSM.vhdl", "w");
	fopen_s(&TB, "FSM_tb.vhdl", "w");

	make_VHDL(VHDL, num_states, num_inputs, num_outputs);
	make_Testbench(TB, num_states, num_inputs, num_outputs);

	fclose(VHDL);
	fclose(TB);

	return 0;
}