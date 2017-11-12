#include "make_VHDL.h"

int convert_int_to_bin(int decimal) {
	/*
	* Recursive method which converts an int into its binary representation.
	*/
	if (decimal == 0) {
		return 0;
	} else {
		return (decimal % 2 + 10 * convert_int_to_bin(decimal / 2));
	}
}

int num_digits(int n) {
	/*
	* Returns the length/sig figs of a positive int.
	*/
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

void case_State_Or_Wait(FILE *VHDL, int total_num_states, int state_num, char states[1000][10], char wait_states[1000][10], int wait) {
	/*
	* Creates the transition conditions for the inputted state
	*/
	//total_num_states is the num_states inputted at the start
	//state_num is the iterator for getting the state from states[]
	//wait is used as a bool to see if input is a state or wait state
	char *when = (char *)calloc(30, (sizeof(char) * 30));
	char *if_statement = (char *)calloc(70, (sizeof(char) * 70));

	if (wait == 1) {

		//when statement
		strcat_s(when, 30, "\t\t\t\twhen ");
		strcat_s(when, 30, wait_states[state_num]);
		strcat_s(when, 30, " =>\n");
		fprintf(VHDL, when);
		free(when);

		//State transition logic
		strcat_s(if_statement, 70, "\t\t\t\t\tif (WAIT_SIG = '0') then\n\t\t\t\t\t\tPS <= ");
		if (state_num == total_num_states - 1) {
			strcat_s(if_statement, 70, states[0]);
		} else {
			strcat_s(if_statement, 70, states[state_num + 1]);
		}

		//Z2 output
		strcat_s(if_statement, 70, ";\n");
		fprintf(VHDL, if_statement);
		free(if_statement);

		//end if
		fprintf(VHDL, "\t\t\t\t\tend if;\n");

	} else {

		//when statement
		strcat_s(when, 30, "\t\t\t\twhen ");
		strcat_s(when, 30, states[state_num]);
		strcat_s(when, 30, " =>\n");
		fprintf(VHDL, when);
		free(when);

		//State transition logic
		strcat_s(if_statement, 70, "\t\t\t\t\tif (WAIT_SIG = '1') then\n\t\t\t\t\t\tPS <= ");
		strcat_s(if_statement, 70, wait_states[state_num]);

		//Z2 output
		strcat_s(if_statement, 70, ";\n");
		fprintf(VHDL, if_statement);
		free(if_statement);

		//end if
		fprintf(VHDL, "\t\t\t\t\tend if;\n");
	}
}

void make_VHDL(FILE *VHDL, int num_states, int num_inputs, int num_outputs) {
	/*
	* Creates a VHDL of a FSM based on the inputted parameters. 
	*/

	//Imports libraries
	fprintf(VHDL, "library ieee;\n");
	fprintf(VHDL, "use ieee.std_logic_1164.all;\n\n");

	//Find the length of the largest states binary representation (to find bundle size for Y output)
	int num_bits = num_digits(convert_int_to_bin((num_states*2)-1))-1;

	//Entity description inputs
	fprintf(VHDL, "entity my_fsm is\n\tport\n\t\t(\n\t\t\tWAIT_SIG, CLK, RESET : in std_logic;\n\n");

	char num_to_char[5]; //Temp store for num to char[]

	//Creates the inputs for the stack inputs based on num_inputs
	for (int i = 0; i < num_inputs; i++) {
		char *input = (char *)calloc(60, sizeof(char) * 60);

		strcat_s(input, 60, "\t\t\tIN");
		snprintf(num_to_char, 5, "%d", i);
		strcat_s(input, 60, num_to_char);
		strcat_s(input, 60, " : in std_logic_vector(31 downto 0);");

		if (i == num_inputs - 1) {
			strcat_s(input, 60, "\n\n");
		} else {
			strcat_s(input, 60, "\n");
		}

		fprintf(VHDL, input);
		free(input);
	}

	//Creates the outputs for the stack outputs based on num_outputs
	for (int i = 0; i < num_outputs; i++) {
		char *output = (char *)calloc(60, sizeof(char) * 60);

		strcat_s(output, 60, "\t\t\tOUT");
		snprintf(num_to_char, 5, "%d", i);
		strcat_s(output, 60, num_to_char);
		strcat_s(output, 60, " : out std_logic_vector(31 downto 0);");

		if (i == num_outputs - 1) {
			strcat_s(output, 60, "\n\n");
		}
		else {
			strcat_s(output, 60, "\n");
		}

		fprintf(VHDL, output);
		free(output);
	}

	//Entity description outputs
	fprintf(VHDL, "\t\t\tRD, WR : out std_logic_vector(31 downto 0);\n\t\t\tY : out std_logic_vector(%d downto 0)\n\t\t);\nend my_fsm;\n\n", num_bits);
	
	//Architecture start
	fprintf(VHDL, "architecture fsm_arch of my_fsm is\n\ttype state_type is (");

	char states[1000][10]; //Stores all names of states
	char wait_states[1000][10]; //Stores all names of wait states
	
	//Creates all state names based on number inputted and state enumeration
	for (int i = 0; i < num_states; i++) {
		char *state = (char *)calloc(15, (sizeof(char) * 15));
		char *wait_state = (char *)calloc(15, (sizeof(char) * 15));

		strcat_s(state, 15, "ST");
		snprintf(num_to_char, 5, "%d", i);
		strcat_s(state, 15, num_to_char);
		strcpy_s(states[i], 10, state);
		strcat_s(state, 15, ", ");

		strcat_s(wait_state, 15, "WS");
		strcat_s(wait_state, 15, num_to_char);
		strcpy_s(wait_states[i], 10, wait_state);

		if (i == num_states - 1) {
			strcat_s(wait_state, 15, ");");
		} else {
			strcat_s(wait_state, 15, ", ");
		}
		fprintf(VHDL, state);
		fprintf(VHDL, wait_state);
		free(state);
		free(wait_state);
	}
	fprintf(VHDL, "\n\tsignal PS, NS : state_type;\n");

	//Architecture processes
	//Sync process
	fprintf(VHDL, "begin\n\tsync_proc : process(CLK, NS, RESET)\n\tbegin\n\t\tif (RESET = '1') then PS <= ST0;");
	fprintf(VHDL, "\n\t\telsif (rising_edge(CLK)) then\n\t\t\tcase PS is\n");
	
	//Creates all of the transition conditions for each state. 
	for (int i = 0; i < num_states; i++) {
		case_State_Or_Wait(VHDL, num_states, i, states, wait_states, 0);
		case_State_Or_Wait(VHDL, num_states, i, states, wait_states, 1);
	}

	//When others statement and end of Comb process
	fprintf(VHDL, "\t\t\t\twhen others =>\n\t\t\t\t\t PS <= ST0;\n\t\t\tend case;");
	//end if and Sync process
	fprintf(VHDL, "\n\t\tend if;\n\tend process sync_proc;\n\n");
	
	//Y output based on current state
	fprintf(VHDL, "\twith PS select\n\t\tY <=");

	//Creates binary representation for each state
	int bin_state_num = 0;
	for (int i = 0; i < num_states; i++) {
		for (int z = 0; z < 2; z++) {

			int binary_state = convert_int_to_bin(bin_state_num);
			fprintf(VHDL, " \"");
			for (int j = 0; j < (num_bits - num_digits(binary_state))+1; j++) {
				fprintf(VHDL, "0");
			}

			if (z % 2 == 1) {
				fprintf(VHDL, "%d\" when %s,\n\t\t\t", binary_state, wait_states[i]);
			} else {
				fprintf(VHDL, "%d\" when %s,\n\t\t\t", binary_state, states[i]);
			}

			bin_state_num++;
		}
	}
	
	//Creates when others binary representation
	fprintf(VHDL, " \"");
	for (int i = 0; i < num_bits+1; i++) {
		fprintf(VHDL, "0");
	}

	//End of Architecture
	fprintf(VHDL, "\" when others;\n");
	fprintf(VHDL, "end fsm_arch;");

	fclose(VHDL);
}

void make_Testbench(FILE *TB, int num_states, int num_inputs, int num_outputs) {
	/*
	* Creates a testbench for the FSM VHDL based on the inputted parameters.
	*/

	//Imports libraries
	fprintf(TB, "library ieee;\n");
	fprintf(TB, "use ieee.std_logic_1164.all;\n\n");

	//Entity description
	fprintf(TB, "entity my_fsm_tb is\nend my_fsm_tb;\n\n");

	int num_bits = num_digits(convert_int_to_bin((num_states * 2) - 1)) - 1;

	//Architecture and Component initialisation
	fprintf(TB, "architecture test of my_fsm_tb is\n\tcomponent my_fsm\n\t\tport\n\t\t(");
	fprintf(TB, "\n\t\t\tWAIT_SIG, RESET, CLK : in std_logic;\n\n");

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
		strcat_s(input, 60, " : in std_logic_vector(31 downto 0);");

		if (i == num_inputs - 1) {
			strcat_s(input, 60, "\n\n");
		}
		else {
			strcat_s(input, 60, "\n");
		}

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
		strcat_s(output, 60, " : out std_logic_vector(31 downto 0);");

		if (i == num_outputs - 1) {
			strcat_s(output, 60, "\n\n");
		}
		else {
			strcat_s(output, 60, "\n");
		}

		fprintf(TB, output);
		free(output);
		free(output_state);
	}

	fprintf(TB, "\t\t\tRD, WR : out std_logic_vector(31 downto 0);\n\t\t\tY : out std_logic_vector(%d downto 0)\n\t\t);\n", num_bits);
	fprintf(TB, "\tend component;\n\n\tsignal WAIT_SIG, RESET, CLK : std_logic;\n\tsignal Y : std_logic_vector(%d downto 0);\n\tsignal ", num_bits);

	for (int i = 0; i < num_inputs; i++) {
		fprintf(TB, "%s, ", inputs[i]);
	}
	for (int i = 0; i < num_outputs; i++) {
		fprintf(TB, "%s", outputs[i]);
		if (i != num_outputs - 1) {
			fprintf(TB, ", ");
		}
	}
	fprintf(TB, ", RD, WR : std_logic_vector(31 downto 0);\n\n");

	//Architecture Processes
	//Port map
	fprintf(TB, "begin\n\tFSM : my_fsm port map (WAIT_SIG => WAIT_SIG, RESET => RESET, CLK => CLK, Y => Y, RD => RD, WR => WR, ");

	for (int i = 0; i < num_inputs; i++) {
		fprintf(TB, "%s => %s, ", inputs[i], inputs[i]);
	}
	for (int i = 0; i < num_outputs; i++) {
		fprintf(TB, "%s => %s", outputs[i], outputs[i]);
		if (i != num_outputs - 1) {
			fprintf(TB, ", ");
		}
	}
	fprintf(TB, ");\n\n\t");

	//CLK process
	fprintf(TB, "CLK_ticks : process\n\tbegin\n\t\tloop\n\t\t\tCLK <= '1';\n\t\t\twait for 5 ns;\n\t\t\t");
	fprintf(TB, "CLK <= '0';\n\t\t\twait for 5 ns;\n\t\tend loop;\n\tend process;\n\n\t");

	//WAIT_SIG and RESET process
	fprintf(TB, "Inputs: process\n\tbegin\n\t\tWAIT_SIG <= '0' after 0 ns,\n\t\t");
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
