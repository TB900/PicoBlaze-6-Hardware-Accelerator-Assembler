#include <stdio.h>
#include <stdlib.h>
#include "make_VHDL.h"

int main(int argc, char *argv[]) {

	if (argc != 2) {
		fprintf(stderr, "Invalid Argument: Please enter the number states you'd like!");
		return 0;
	}

	int num_states = *argv[1] - '0';

	FILE* VHDL;
	FILE* TB;

	fopen_s(&VHDL, "FSM.vhdl", "w");
	fopen_s(&TB, "FSM_tb.vhdl", "w");

	make_VHDL(VHDL, num_states);
	make_Testbench(TB, num_states);

	fclose(VHDL);
	fclose(TB);

	return 0;
}