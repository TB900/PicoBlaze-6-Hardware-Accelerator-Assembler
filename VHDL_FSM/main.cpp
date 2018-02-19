#include <stdio.h>
#include <stdlib.h>
#include "make_VHDL.h"
#include "parser.h"

int main(int argc, char *argv[]) {

	// Uses default assembly file name and name if one isn't entered
	if (argc != 3) {
		argv[1] = "assembly.psm";
		argv[2] = "HACC";
	}

	// Create a dataflow linked list structure for storing the assembly instructions
	dataflow *assembly = NULL;
	assembly = (dataflow *)malloc(sizeof(dataflow));
	assembly->next = NULL;

	// Parse the assembly file into the dataflow linked list where the number of states and last mem instruction are assigned
	int num_ST;
	instruction last_ins;
	parse_assembly(argv[1], &assembly, &num_ST, &last_ins);

	// Debug printing function
	print_dataflow(&assembly);
	
	// Initialise num inputs, outputs, file pointers for VHDL and testbench and filenames for the VHDL files
	int num_IN = 0, num_OUT = 0;
	FILE *VHDL, *TB;
	char *comp_filename = (char *)calloc(100, sizeof(char) * 100);
	strcat_s(comp_filename, 100, argv[2]);
	strcat_s(comp_filename, 100, ".vhdl");
	char *test_filename = (char *)calloc(105, sizeof(char) * 105);
	strcat_s(test_filename, 105, argv[2]);
	strcat_s(test_filename, 105, "_tb.vhdl");

	// Open the files to write
	fopen_s(&VHDL, comp_filename, "w");
	fopen_s(&TB, test_filename, "w");

	// Functions which create the VHDL componenets and testbench
	make_VHDL(VHDL, argv[2], num_ST, num_IN, num_OUT, last_ins, &assembly);
	make_Testbench(TB, argv[2], num_ST, num_IN, num_OUT);

	// Closes the files and ends 
	fclose(VHDL);
	fclose(TB);

	return 0;
}