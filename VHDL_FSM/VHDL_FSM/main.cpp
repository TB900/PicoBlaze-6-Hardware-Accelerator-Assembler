#include "main.h"

int main(int argc, char *argv[]) {

	// When no arguments entered return an error
	if (argc <= 1) {
		fprintf(stderr, "Error: No arguments supplied");
		return -1;

	// If just a file name is entered
	} else if (argc == 2) {
		argv[2] = "HACC";
		argv[3] = "ON";
		argv[4] = "0";
		argv[5] = "0";
	
	// If just a file name and entity name is entered
	} else if (argc == 3) {
		argv[3] = "ON";
		argv[4] = "0";
		argv[5] = "0";

	// If a file name, entity name and sleep switch is entered
	} else if (argc == 4) {
		argv[4] = "0";
		argv[5] = "0";

	// If a file name, entity name, sleep switch and number of inputs entered
	} else if (argc == 5) {
		argv[5] = "0";
	
	// If more than 5 arguments provided, exit the program
	} else if (argc > 6) {
		fprintf(stderr, "Error: More than 5 arguments given");
		return -1;
	}

	// Check the entity name starts with a letter and only uses letters, numbers and underscores
	int entity_size = 0;
	
	if ((!isalpha(argv[2][entity_size]))) {
		fprintf(stderr, "Error: 2nd argument for entity name is invalid! Make sure the name begins with a letter!");
		return -1;
	}

	while (argv[2][entity_size] != '\0') {
		if ((!isalpha(argv[2][entity_size])) && (!isdigit(argv[2][entity_size])) && (argv[2][entity_size] != '_')) {
			fprintf(stderr, "Error: 2nd argument for entity name is invalid! Make sure the name only contains letters, numbers and underscores!");
			return -1;
		}
		entity_size++;
	}

	// Check the sleep switch argument is either 1 or 0
	if ((strcmp(argv[3], "ON") != 0) && (strcmp(argv[3], "OFF") != 0)) {
		fprintf(stderr, "Error: 3rd argument for sleep switch is invalid! Make sure it is either ON or OFF!");
		return -1;
	}

	// Set int values for off and on
	int pb_sleep;
	if (strcmp(argv[3], "OFF") == 0) {
		pb_sleep = 0;
	} else {
		pb_sleep = 1;
	}

	// Check the number of inputs and outputs are numbers within long int range
	char *endptr = 0;
	errno = 0;
	long int num_IN = strtol(argv[4], &endptr, 10);
	if (errno == ERANGE) {
		fprintf(stderr, "Error: 4th argument for number of inputs is invalid! Make sure it is a valid long int value!");
		return -1;
	} else if (endptr == argv[4]) {
		fprintf(stderr, "Error: 4th argument for number of inputs is invalid! Make sure it is a valid long int value!");
		return -1;
	}

	endptr = 0;
	errno = 0;
	long int num_OUT = strtol(argv[5], &endptr, 10);
	if (errno == ERANGE) {
		fprintf(stderr, "Error: 5th argument for number of inputs is invalid! Make sure it is a valid long int value!");
		return -1;
	} else if (endptr == argv[5]) {
		fprintf(stderr, "Error: 5th argument for number of inputs is invalid! Make sure it is a valid long int value!");
		return -1;
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
	int num_bits = make_VHDL(VHDL, argv[2], num_ST, pb_sleep, num_IN, num_OUT, last_ins, &assembly);
	make_Testbench(TB, argv[2], num_ST, num_IN, num_OUT, num_bits);

	// Closes the files and ends 
	fclose(VHDL);
	fclose(TB);

	return 0;
}