#include "make_VHDL.h"

int convert_int_to_bin(int decimal) {
	if (decimal == 0) {
		return 0;
	} else {
		return (decimal % 2 + 10 * convert_int_to_bin(decimal / 2));
	}
}

int num_digits(int n) {
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

void make_VHDL(FILE *VHDL, int num_states) {

	fprintf(VHDL, "library ieee;\n");
	fprintf(VHDL, "use ieee.std_logic_1164.all;\n\n");

	int num_bits = num_digits(convert_int_to_bin(num_states-1));

	fprintf(VHDL, "entity my_fsm is\n\t port ( WAIT, CLK, RESET : in std_logic;\n\t\t\tY : out std_logic_vector(%d downto 0);\n\t\t\tZ1, Z2 : out std_logic);\nend my_fsm;\n\n", num_bits);
	
	fprintf(VHDL, "architecture fsm_arch of my_fsm is\n\ttype state_type is (");

	char ST_num[4];
	char states[1000][10];
	
	for (int i = 0; i < num_states; i++) {
		char *state = (char *)calloc(15, (sizeof(char) * 15));
		strcat_s(state, 15, "ST");
		snprintf(ST_num, 4, "%d", i);
		strcat_s(state, 15, ST_num);
		strcpy_s(states[i], 10, state);
		if (i == num_states - 1) {
			strcat_s(state, 15, ");");
		} else {
			strcat_s(state, 15, ", ");
		}
		fprintf(VHDL, state);
		free(state);
	}

	fprintf(VHDL, "\n\tsignal PS, NS : state_type;\n");
	fprintf(VHDL, "begin\n\tsync_proc : process(CLK, NS, RESET)\n\tbegin\n\t\tif (RESET = '1') then PS <= ST0;");
	fprintf(VHDL, "\n\t\telsif (rising_edge(CLK)) then PS <= NS;\n\t\tend if;\n\tend process sync_proc;\n\n");
	fprintf(VHDL, "\tcomb_proc : process(PS, WAIT)\n\tbegin\n\t\tZ1 <= '0'; Z2 <= '0';\n\t\tcase PS is\n");

	for (int i = 0; i < num_states; i++) {
		char *when = (char *)calloc(30, (sizeof(char) * 30));
		char *Z1_output = (char *)calloc(20, (sizeof(char) * 20));
		char *if_statement = (char *)calloc(55, (sizeof(char) * 55));
		char *else_statement = (char *)calloc(45, (sizeof(char) * 45));

		strcat_s(when, 30, "\t\t\twhen ");
		strcat_s(when, 30, states[i]);
		strcat_s(when, 30, " =>\n");
		fprintf(VHDL, when);
		free(when);

		strcat_s(Z1_output, 20, "\t\t\t\tZ1 <= '0';\n");
		fprintf(VHDL, Z1_output);
		free(Z1_output);

		strcat_s(if_statement, 55, "\t\t\t\tif (WAIT = '0') then\n\t\t\t\t\tNS <= ");
		if (i == num_states - 1) {
			strcat_s(if_statement, 55, states[0]);
		} else {
			strcat_s(if_statement, 55, states[i+1]);
		}
		strcat_s(if_statement, 55, "; Z2 <= '0';\n");
		fprintf(VHDL, if_statement);
		free(if_statement);

		strcat_s(else_statement, 45, "\t\t\t\telse\n\t\t\t\t\tNS <= ");
		strcat_s(else_statement, 45, states[i]);
		strcat_s(else_statement, 45, "; Z2 <= '1';\n");
		fprintf(VHDL, else_statement);
		free(else_statement);

		fprintf(VHDL, "\t\t\t\tend if;\n");
	}

	fprintf(VHDL, "\t\t\twhen others =>\n\t\t\t\tZ1 <= '0'; Z2 <= '0'; NS <= ST0;\n\t\tend case;\n\tend process comb_proc;\n\n");
	fprintf(VHDL, "\twith PS select\n\t\tY <=");

	for (int i = 0; i < num_states; i++) {
		int binary_state = convert_int_to_bin(i);
		fprintf(VHDL, " \"");
		for (int j = 0; j < (num_bits - num_digits(binary_state)); j++) {
			fprintf(VHDL, "0");
		}

		fprintf(VHDL, "%d\" when %s,\n\t\t\t", convert_int_to_bin(i), states[i]);
	}
	
	fprintf(VHDL, " \"");
	for (int i = 0; i < num_bits; i++) {
		fprintf(VHDL, "0");
	}

	fprintf(VHDL, "\" when others;\n");
	fprintf(VHDL, "end fsm_arch;");

	fclose(VHDL);
}

void make_Testbench(FILE *TB, int num_states) {

	//Imports libraries
	fprintf(TB, "library ieee;\n");
	fprintf(TB, "use ieee.std_logic_1164.all;\n\n");

	//Entity description
	fprintf(TB, "entity my_fsm_tb is\nend my_fsm_tb;\n\n");

	//Architecture and Component initialisation
	fprintf(TB, "architecture test of my_fsm_tb is\n\tcomponent my_fsm\n\t\tport\n\t\t(");
	fprintf(TB, "\n\t\t\tWAIT, RESET, CLK : in std_logic;\n\t\t\tZ1, Z2 : out std_logic;\n\t\t\tY : out std_logic_vector(1 downto 0)\n\t\t);\n");
	fprintf(TB, "\tend component;\n\n\tsignal WAIT, RESET, CLK, Z1, Z2 : std_logic;\n\tsignal Y : std_logic_vector(1 downto 0);\n\n");

	//Architecture Processes
	//Port map
	fprintf(TB, "begin\n\tFSM : my_fsm port map (WAIT => WAIT, RESET => RESET, CLK => CLK, Z1 => Z1, Z2 => Z2, Y => Y);\n\n\t");

	//CLK process
	fprintf(TB, "CLK_ticks : process\n\tbegin\n\t\tloop\n\t\t\tCLK <= '1';\n\t\t\twait for 5 ns;\n\t\t\t");
	fprintf(TB, "CLK <= '0';\n\t\t\twait for 5 ns;\n\t\tend loop;\n\tend process;\n\n\t");

	//WAIT and RESET process
	fprintf(TB, "Inputs: process\n\tbegin\n\t\tWAIT <= '1' after 0 ns,\n\t\t");
	int interval = 10;
	for (int i = 1; i < num_states + 1; i++) {
		fprintf(TB, "'0' after %d ns,\n\t\t", interval);
		interval += 10;
		fprintf(TB, "'1' after %d ns,\n\t\t", interval);
		interval += 10;
	}

	fprintf(TB, "\n\t\tassert false report \"Reached end of text\";\n\t\twait;\n\n\tend process;\nend test;");

	fclose(TB);
}
