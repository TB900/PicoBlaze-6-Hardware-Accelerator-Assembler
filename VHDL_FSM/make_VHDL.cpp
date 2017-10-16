#include "make_VHDL.h"

void make_VHDL(FILE *VHDL, int num_states) {

	fprintf(VHDL, "library ieee;\n");
	fprintf(VHDL, "use ieee.std_logic_1164.all;\n\n");

	fprintf(VHDL, "entity my_fsm is\n\t port ( X, CLK, RESET : in std_logic;\n\t\t\tY : out std_logic_vector(1 downto 0);\n\t\t\tZ1, Z2 : out std_logic);\nend my_fsm;\n\n");
	
	fprintf(VHDL, "architecture fsm_arch of my_fsm is\n\ttype state_type is (");

	char ST_num[4];
	char states[100][10];
	
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
	fprintf(VHDL, "begin\n\tsync_proc : process(CLK, NS, RESET)\n\tbegin\n\t\tif (RESET = '1') then PS <= ST0");
	fprintf(VHDL, "\n\t\telsif (rising_edge(CLK)) then PS <= NS;\n\t\tend if;\n\tend process sync_proc;\n\n");
	fprintf(VHDL, "\tcomb_proc : process(PS, X)\n\tbegin\n\t\tZ1 <= '0'; Z2 <= '0';\n\t\tcase PS is\n");

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

		strcat_s(if_statement, 55, "\t\t\t\tif (X = '0') then\n\t\t\t\t\tNS <= ");
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
		fprintf(VHDL, " '%d' when %s,\n\t\t\t", i, states[i]);
	}

	fprintf(VHDL, " '0' when others;\n");
	fprintf(VHDL, "end my_fsm;");

	fclose(VHDL);
}
