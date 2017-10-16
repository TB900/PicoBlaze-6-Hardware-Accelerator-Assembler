#include <stdio.h>
#include <stdlib.h>
#include "make_VHDL.h"

int main() {
	FILE* VHDL;

	fopen_s(&VHDL, "my_VHDL_FSM.vhdl", "w");

	make_VHDL(VHDL, 4);
	
	fclose(VHDL);
}