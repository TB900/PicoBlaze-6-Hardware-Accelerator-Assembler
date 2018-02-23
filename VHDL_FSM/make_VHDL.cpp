#include "make_VHDL.h"

// Recursive method which converts an int into its binary representation.
static int convert_int_to_bin(int decimal) {
	if (decimal == 0) {
		return 0;
	} else {
		return (decimal % 2 + 10 * convert_int_to_bin(decimal / 2));
	}
}

// Returns the length/sig figs of a positive int.
static int num_digits(int n) {
	if (n < 10) { return 1; }
	else if (n < 100) { return 2; }
	else if (n < 1000) { return 3; }
	else if (n < 10000) { return 4; }
	else if (n < 100000) { return 5; }
	else if (n < 1000000) { return 6; }
	else if (n < 10000000) { return 7; }
	else if (n < 100000000) { return 8; }
	else if (n < 1000000000) { return 9; }
	else { return 10; }
}

// Creates the transition conditions for the inputted state
static void case_State_Or_Wait(FILE *VHDL, int state_num, char states[1000][10], int write_active) {
	
	// total_num_states is the num_states inputted at the start
	// state_num is the iterator for getting the state from states[]
	// wait is used as a bool to see if input is a state or wait state
	char *when = (char *)calloc(30, (sizeof(char) * 30));
	char *if_statement = (char *)calloc(100, (sizeof(char) * 100));

	// If the state is a wait state
	if (strstr(states[state_num], "WS") != NULL) {

		// when statement
		strcat_s(when, 30, "\t\t\t\twhen ");
		strcat_s(when, 30, states[state_num]);
		strcat_s(when, 30, " =>\n");
		fprintf(VHDL, when);
		free(when);

		// Request Write or Read
		char *request = (char *)calloc(15, (sizeof(char) * 15));
		if (write_active == 1) {
			strcat_s(request, 20, "WR_RQ <= ");
		} else {
			strcat_s(request, 20, "RD_RQ <= ");
		}

		// Request on
		fprintf(VHDL, "\t\t\t\t\t");
		fprintf(VHDL, request);
		fprintf(VHDL, "'1';\n");

		// State transition logic and Request off
		strcat_s(if_statement, 100, "\t\t\t\t\tif (MEM_GNT = '0') then");
		strcat_s(if_statement, 100, "\n\t\t\t\t\t\t");
		strcat_s(if_statement, 100, request);
		strcat_s(if_statement, 100, "'0';");
		strcat_s(if_statement, 100,	"\n\t\t\t\t\t\tPS <= ");
		strcat_s(if_statement, 100, states[state_num + 1]);

		// Finish statement and print to file
		strcat_s(if_statement, 100, ";\n");
		fprintf(VHDL, if_statement);
		free(request);
		free(if_statement);

		// end if
		fprintf(VHDL, "\t\t\t\t\tend if;\n\n");

	} else {

		// when statement
		strcat_s(when, 30, "\t\t\t\twhen ");
		strcat_s(when, 30, states[state_num]);
		strcat_s(when, 30, " =>\n");
		fprintf(VHDL, when);
		free(when);

		// If the next state is the final state, don't request the memory and just proceed to ST_END
		if (strcmp(states[state_num + 1], "ST_END") == 0) {
			fprintf(VHDL, "\t\t\t\t\tPS <= ST_END; \n\n");
			free(if_statement);

		} else {

			// State transition logic and MEM Request
			strcat_s(if_statement, 100, "\t\t\t\t\tMEM_RQ <= '1';");
			strcat_s(if_statement, 100, "\n\t\t\t\t\tif (MEM_GNT = '1') then\n\t\t\t\t\t\tMEM_RQ <= '0';\n\t\t\t\t\t\tPS <= ");
			strcat_s(if_statement, 100, states[state_num + 1]);

			// Finish statement and print to file
			strcat_s(if_statement, 100, ";\n");
			fprintf(VHDL, if_statement);
			free(if_statement);

			// end if
			fprintf(VHDL, "\t\t\t\t\tend if;\n\n");
		}
	}
}

static void action_Proc(FILE *VHDL, dataflow **assembly, char states[1000][10], int length_of_states) {
	dataflow *current_ins = *assembly; // Contains the node of the current instruction
	int current_state = 1; // Used as an index for the states array 

	fprintf(VHDL, "\t\t\twhen %s =>\n", states[current_state]);

	// While the current instruction isn't NULL
	while (current_ins->next != NULL) {
		current_ins = current_ins->next;

		switch (current_ins->ins) {
			case(ADD):
				ins_ADD(VHDL, current_ins->op1, current_ins->op2);
				break;

			case(STORE):
				ins_WRITE(VHDL, current_ins->ins, current_ins->op1, current_ins->op2, states[current_state + 1], states[current_state + 2]);
				current_state += 2;
				break;

			case(FETCH):
				ins_READ(VHDL, current_ins->ins, current_ins->op1, current_ins->op2, states[current_state + 1], states[current_state + 2]);
				current_state += 2;
				break;

			case(SL0):
				ins_SL0(VHDL, current_ins->op1);
				break;
			
			case(INPUT):
				ins_READ(VHDL, current_ins->ins, current_ins->op1, current_ins->op2, states[current_state + 1], states[current_state + 2]);
				current_state += 2;
				break;

			case(OUTPUT):
				ins_WRITE(VHDL, current_ins->ins, current_ins->op1, current_ins->op2, states[current_state + 1], states[current_state + 2]);
				current_state += 2;
				break;

			case(SL1):
				ins_SL1(VHDL, current_ins->op1);
				break;

			case(SLA):
				ins_SLA(VHDL, current_ins->op1);
				break;

			case(SLX):
				ins_SLX(VHDL, current_ins->op1);
				break;

			case(SR0):
				ins_SR0(VHDL, current_ins->op1);
				break;

			case(SR1):
				ins_SR1(VHDL, current_ins->op1);
				break;

			case(SRA):
				ins_SRA(VHDL, current_ins->op1);
				break;

			case(SRX):
				ins_SRX(VHDL, current_ins->op1);
				break;

			case(RL):
				ins_RL(VHDL, current_ins->op1);
				break;

			case(RR):
				ins_RR(VHDL, current_ins->op1);
				break;

			case(ADDCY):
				ins_ADDCY(VHDL, current_ins->op1, current_ins->op2);
				break;

			case(SUB):
				ins_SUB(VHDL, current_ins->op1, current_ins->op2);
				break;

			case(SUBCY):
				ins_SUBCY(VHDL, current_ins->op1, current_ins->op2);
				break;

			case(AND):
				ins_AND(VHDL, current_ins->op1, current_ins->op2);
				break;

			case(OR):
				ins_OR(VHDL, current_ins->op1, current_ins->op2);
				break;

			case(XOR):
				ins_XOR(VHDL, current_ins->op1, current_ins->op2);
				break;

			case(LOAD):
				ins_LOAD(VHDL, current_ins->op1, current_ins->op2);
				break;

			case(COMPARE):
				ins_COMPARE(VHDL, current_ins->op1, current_ins->op2);
				break;

			case(COMPARECY):
				ins_COMPARECY(VHDL, current_ins->op1, current_ins->op2);
				break;

			case(TEST):
				ins_TEST(VHDL, current_ins->op1, current_ins->op2);
				break;

			case(TESTCY):
				ins_TESTCY(VHDL, current_ins->op1, current_ins->op2);
				break;
		}
	}
}

// Creates a VHDL of a FSM based on the inputted parameters. 
void make_VHDL(FILE *VHDL, char *entity, int num_states, int pb_sleep,int num_inputs, int num_outputs, instruction last_ins, dataflow **assembly) {

	//Imports libraries
	fprintf(VHDL, "library ieee;\n");
	fprintf(VHDL, "use ieee.std_logic_1164.all;\n");
	fprintf(VHDL, "use ieee.numeric_std.all;\n\n");

	//Find the length of the largest states binary representation (to find bundle size for Y output)
	int num_bits;
	if (last_ins == 0) {
		num_bits = num_digits(convert_int_to_bin((num_states * 2) + 1))-1;
	} else {
		num_bits = num_digits(convert_int_to_bin((num_states * 2)))-1;
	}
	//Entity description inputs
	fprintf(VHDL, "entity %s is\n\tport\n\t\t(\n\t\t\tCLK, START, MEM_GNT : in std_logic;\n", entity);

	char num_to_char[5]; //Temp store for num to char[]

	//Creates the inputs for the stack inputs based on num_inputs
	//No longer needed so when asked how many inputs, 0 is used
	for (int i = 0; i < num_inputs; i++) {
		char *input = (char *)calloc(60, sizeof(char) * 60);

		strcat_s(input, 60, "\t\t\tIN");
		snprintf(num_to_char, 5, "%d", i);
		strcat_s(input, 60, num_to_char);
		strcat_s(input, 60, " : in std_logic_vector(7 downto 0);");

		strcat_s(input, 60, "\n");

		fprintf(VHDL, input);
		free(input);
	}

	//Creates the outputs for the stack outputs based on num_outputs
	//No longer needed so when asked how many inputs, 0 is used
	for (int i = 0; i < num_outputs; i++) {
		char *output = (char *)calloc(60, sizeof(char) * 60);

		strcat_s(output, 60, "\t\t\tOUT");
		snprintf(num_to_char, 5, "%d", i);
		strcat_s(output, 60, num_to_char);
		strcat_s(output, 60, " : out std_logic_vector(7 downto 0);");

		strcat_s(output, 60, "\n");

		fprintf(VHDL, output);
		free(output);
	}

	//Entity description INPUT , OUTPUT, Request Outputs and Current State Number output
	fprintf(VHDL, "\t\t\tDATA_IN : in std_logic_vector(7 downto 0);\n\t\t\tDATA_OUT, ADDR : out std_logic_vector(7 downto 0);");
	fprintf(VHDL, "\n\t\t\tMEM_RQ, RD_RQ, WR_RQ, PB_SLEEP : out std_logic;\n\t\t\tY : out std_logic_vector(%d downto 0)\n\t\t);\nend %s;\n\n", num_bits, entity);
	
	//Architecture start
	fprintf(VHDL, "architecture HACC_arch of %s is\n\ttype state_type is (", entity);

	char states[1000][10]; //Stores all names of states

	// Set and print initialisation state name
	strcpy_s(states[0], 10, "ST_START");
	fprintf(VHDL, states[0]);
	fprintf(VHDL, ", ");
	
	//Creates all state names based on number inputted and state enumeration calculating the true number of states and wait states
	int i;
	int len_of_states = 1;
	for (i = 0; i < num_states; i++, len_of_states +=2) {
		char *state = (char *)calloc(15, (sizeof(char) * 15));
		char *wait_state = (char *)calloc(15, (sizeof(char) * 15));

		strcat_s(state, 15, "ST");
		snprintf(num_to_char, 5, "%d", i);
		strcat_s(state, 15, num_to_char);
		strcpy_s(states[len_of_states], 10, state);
		strcat_s(state, 15, ", "); 

		if (i == num_states - 1 && (last_ins == FETCH || last_ins == INPUT)) {
			fprintf(VHDL, state);
			free(state);
			free(wait_state);
			len_of_states++;
			break;
		}

		strcat_s(wait_state, 15, "WS");
		strcat_s(wait_state, 15, num_to_char);
		strcpy_s(states[len_of_states +1], 10, wait_state);
		strcat_s(wait_state, 15, ", ");

		fprintf(VHDL, state);
		fprintf(VHDL, wait_state);
		free(state);
		free(wait_state);
	}

	// Set and print end state name
	strcpy_s(states[len_of_states], 10, "ST_END");
	fprintf(VHDL, states[len_of_states]);
	fprintf(VHDL, ");");
	len_of_states += 1; // For the extra end states

	fprintf(VHDL, "\n\tsignal PS : state_type;\n");

	//Architecture processes
	//Sync process
	fprintf(VHDL, "begin\n\tsync_proc : process(CLK, PS, START, MEM_GNT)\n\tbegin\n\t\t");
	fprintf(VHDL, "if (START = '1') then\n\t\t\tPB_SLEEP <= '%i';\n\t\tend if;", pb_sleep);
	fprintf(VHDL, "\n\n\t\tif (rising_edge(CLK)) then\n\t\t\tcase PS is\n");
	
	// ST_START case
	fprintf(VHDL, "\t\t\t\twhen ST_START =>\n\t\t\t\t\tif (START = '1') then\n\t\t\t\t\t\t");
	fprintf(VHDL, "PS <= ST0;\n\t\t\t\t\telse \n\t\t\t\t\t\tPB_SLEEP <= '0';\n\t\t\t\t\t\t");
	fprintf(VHDL, "PS <= ST_START\n\t\t\t\t\tend if;\n\n");

	//Creates all of the transition conditions for each state along with the RD and WR Requests.
	dataflow *mem_ins = *assembly;
	mem_ins = mem_ins->next;
	int write_active = 0;
	for (i = 1; i < len_of_states-1; i++) {

		// If the state is a wait state, need to find out whether the instruction is a read or write
		if (strstr(states[i], "WS") != NULL) {

			while (mem_ins->next != NULL) { // NEED TO ADD INPUT AND OUTPUT ENUMERATIONS
				if (mem_ins->ins == STORE || mem_ins->ins == FETCH || mem_ins->ins == INPUT || mem_ins->ins == OUTPUT) {
					break;
				}
				mem_ins = mem_ins->next;
			}

			// If the corresponding wait instruction is a writing MEM operation, set wait to 1 else 0
			if (mem_ins->ins == STORE || mem_ins->ins == OUTPUT) {
				write_active = 1;
			} else {
				write_active = 0;
			}

			// Advance to next instruction for next loop and complete the WS implementation
			mem_ins = mem_ins->next;
			case_State_Or_Wait(VHDL, i, states, write_active);

		} else {

			case_State_Or_Wait(VHDL, i, states, write_active);

		}
	}

	// ST_END case
	fprintf(VHDL, "\t\t\t\twhen ST_END =>\n\t\t\t\t\tPB_SLEEP <= '0';\n\t\t\t\t\tPS <= ST_START;\n\n");

	//When others statement and end of case
	fprintf(VHDL, "\t\t\t\twhen others =>\n\t\t\t\t\tPS <= ST_START;\n\n\t\t\tend case;");
	//end if and Sync process
	fprintf(VHDL, "\n\t\tend if;\n\tend process sync_proc;\n\n");

	// ACTION process variable initialisation and begin case for PS
	fprintf(VHDL, "\taction_proc : process(PS)\n\t\tvariable s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF : unsigned(7 downto 0);");
	fprintf(VHDL, "\n\t\tvariable TMP : unsigned(8 downto 0);\n\t\tvariable CARRY, ZERO : std_logic;\n\tbegin\n\t\tcase (PS) is\n");

	// Create each case for PS
	action_Proc(VHDL, assembly, states, len_of_states);

	// when others statement and end of case
	fprintf(VHDL, "\t\t\twhen others =>\n\t\t\t\tnull;\n\n\t\tend case;");
	// end of Action process
	fprintf(VHDL, "\n\tend process action_proc;\n\n");
	
	//Y output based on current state
	fprintf(VHDL, "\twith PS select\n\t\tY <=");

	//Creates binary representation for each state
	int bin_state_num = 0;
	for (i = 0; i < len_of_states; i++) {

		int binary_state = convert_int_to_bin(bin_state_num);
		fprintf(VHDL, " \"");
		for (int j = 0; j < (num_bits - num_digits(binary_state))+1; j++) {
			fprintf(VHDL, "0");
		}

		fprintf(VHDL, "%d\" when %s,\n\t\t\t", binary_state, states[i]);

		bin_state_num++;
	}
	
	//Creates when others binary representation
	fprintf(VHDL, " \"");
	for (i = 0; i < num_bits+1; i++) {
		fprintf(VHDL, "0");
	}

	//End of Architecture
	fprintf(VHDL, "\" when others;\n");
	fprintf(VHDL, "end HACC_arch;");

	fclose(VHDL);
}

// Creates a testbench for the FSM VHDL based on the inputted parameters.
void make_Testbench(FILE *TB, char *entity, int num_states, int num_inputs, int num_outputs) {

	//Imports libraries
	fprintf(TB, "library ieee;\n");
	fprintf(TB, "use ieee.std_logic_1164.all;\n\n");

	//Entity description
	fprintf(TB, "entity %s_tb is\nend %s_tb;\n\n", entity, entity);

	int num_bits = num_digits(convert_int_to_bin((num_states * 2) - 1)) - 1;

	//Architecture and Component initialisation
	fprintf(TB, "architecture test of %s_tb is\n\tcomponent my_fsm\n\t\tport\n\t\t(", entity);
	fprintf(TB, "\n\t\t\tMEM_GNT, START, CLK : in std_logic;\n");

	char num_to_char[5]; //Temp store for num to char[]
	char inputs[100][10];
	char outputs[100][10];

	//Creates the inputs for the stack inputs based on num_inputs
	for (int i = 0; i < num_inputs; i++) {
		char *input = (char *)calloc(60, sizeof(char) * 60);
		char *input_state = (char *)calloc(10, sizeof(char) * 10);

		strcat_s(input, 60, "\t\t\t");
		strcat_s(input_state, 10, "IN");
		snprintf(num_to_char, 5, "%d", i);
		strcat_s(input_state, 10, num_to_char);
		strcpy_s(inputs[i], 10, input_state);
		strcat_s(input, 60, input_state);
		strcat_s(input, 60, " : in std_logic_vector(7 downto 0);");

		strcat_s(input, 60, "\n");

		fprintf(TB, input);
		free(input);
		free(input_state);
	}

	//Creates the outputs for the stack outputs based on num_outputs
	for (int i = 0; i < num_outputs; i++) {
		char *output = (char *)calloc(60, sizeof(char) * 60);
		char *output_state = (char *)calloc(10, sizeof(char) * 10);

		strcat_s(output, 60, "\t\t\t");
		strcat_s(output_state, 10, "OUT");
		snprintf(num_to_char, 5, "%d", i);
		strcat_s(output_state, 10, num_to_char);
		strcpy_s(outputs[i], 10, output_state);
		strcat_s(output, 60, output_state);
		strcat_s(output, 60, " : out std_logic_vector(7 downto 0);");

		strcat_s(output, 60, "\n");

		fprintf(TB, output);
		free(output);
		free(output_state);
	}

	fprintf(TB, "\t\t\tRD_RQ, WR_RQ : out std_logic;\n\t\t\tY : out std_logic_vector(%d downto 0)\n\t\t);\n", num_bits);
	fprintf(TB, "\tend component;\n\n\tsignal MEM_GNT, START, CLK, RD_RQ, WR_RQ : std_logic;\n\tsignal Y : std_logic_vector(%d downto 0);\n\tsignal ", num_bits);

	if (num_inputs > 0 && num_outputs > 0) {
		for (int i = 0; i < num_inputs; i++) {
			fprintf(TB, "%s, ", inputs[i]);
		}
		for (int i = 0; i < num_outputs; i++) {
			fprintf(TB, "%s", outputs[i]);
			if (i != num_outputs - 1) {
				fprintf(TB, ", ");
			}
		}
		fprintf(TB, " : std_logic_vector(7 downto 0);\n\n");
	}

	//Architecture Processes
	//Port map
	fprintf(TB, "begin\n\tFSM : my_fsm port map (MEM_GNT => MEM_GNT, START => START, CLK => CLK, Y => Y, RD_RQ => RD_RQ, WR_RQ => WR_RQ");

	for (int i = 0; i < num_inputs; i++) {
		fprintf(TB, ", %s => %s", inputs[i], inputs[i]);
	}
	for (int i = 0; i < num_outputs; i++) {
		fprintf(TB, ", %s => %s", outputs[i], outputs[i]);
	}
	fprintf(TB, ");\n\n\t");

	//CLK process
	fprintf(TB, "CLK_ticks : process\n\tbegin\n\t\tloop\n\t\t\tCLK <= '1';\n\t\t\twait for 2.5 ns;\n\t\t\t");
	fprintf(TB, "CLK <= '0';\n\t\t\twait for 2.5 ns;\n\t\tend loop;\n\tend process;\n\n\t");

	//WAIT_SIG and RESET process
	fprintf(TB, "Inputs: process\n\tbegin\n\t\tSTART <= '1' after 0 ns;\n\t\tMEM_GNT <= '0' after 0 ns,\n\t\t");
	int interval = 10;
	for (int i = 1; i < num_states + 1; i++) {
		fprintf(TB, "'1' after %d ns,\n\t\t", interval);
		interval += 10;
		fprintf(TB, "'0' after %d ns", interval);
		interval += 10;
		if (i != num_states) {
			fprintf(TB, ",\n\t\t");
		} else {
			fprintf(TB, ";\n\t\t");
		}
	}

	//End of test assertion
	fprintf(TB, "\n\t\tassert false report \"Reached end of text\";\n\t\twait;\n\n\tend process;\nend test;");

	fclose(TB);
}
