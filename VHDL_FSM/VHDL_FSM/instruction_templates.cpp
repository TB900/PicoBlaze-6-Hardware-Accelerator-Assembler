#include "instruction_templates.h"

void ins_ADD(FILE *VHDL, char *op1, char *op2) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- ADD %s, %s\n", op1, op2);

	// If both operands are registers
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) + ('0' & %s);\n", op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) + x\"%s\";\n", op1, op2);
	}

	// Set the destination register
	fprintf(VHDL, "\t\t\t\t%s := TMP(7 downto 0);\n", op1);

	// Set the CARRY flag
	fprintf(VHDL, "\t\t\t\tCARRY := TMP(8);\n");

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_READ(FILE *VHDL, instruction ins, char *op1, char *op2, char *wait_state, char *next_state) {
	// Instruction comment
	if (ins == FETCH) {
		fprintf(VHDL, "\t\t\t\t-- FETCH %s, %s\n", op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\t-- INPUT %s, %s\n", op1, op2);
	}

	// Create case for wait state
	fprintf(VHDL, "\t\t\twhen %s =>\n", wait_state);
	
	// Output ADDR with inputted operand 2 depending if register or literal
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\tADDR <= std_logic_vector(%s);\n\n", op2);
	} else {
		fprintf(VHDL, "\t\t\t\tADDR <= x\"%s\";\n\n", op2);
	}

	// Create case for next state
	fprintf(VHDL, "\t\t\twhen %s =>\n", next_state);

	// Retrieve value retrieved from memory on the DATA_IN into operand 1 register
	fprintf(VHDL, "\t\t\t\t%s := unsigned(DATA_IN);\n", op1);

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_WRITE(FILE *VHDL, instruction ins, char *op1, char *op2, char *wait_state, char *next_state) {
	// Instruction comment
	if (ins == STORE) {
		fprintf(VHDL, "\t\t\t\t-- STORE %s, %s\n", op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\t-- OUTPUT %s, %s\n", op1, op2);
	}

	// Create case for wait state
	fprintf(VHDL, "\t\t\twhen %s =>\n", wait_state);

	// Output ADDR with inputted operand 2 depending if register or literal
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\tADDR <= std_logic_vector(%s);\n", op2);
	} else {
		fprintf(VHDL, "\t\t\t\tADDR <= x\"%s\";\n", op2);
	}

	// Output operand 1 on DATA_OUT
	fprintf(VHDL, "\t\t\t\tDATA_OUT <= std_logic_vector(%s);\n\n", op1);

	// Create case for next state
	fprintf(VHDL, "\t\t\twhen %s =>\n", next_state);
}

void ins_SL0(FILE *VHDL, char *op1) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- SL0 %s\n", op1);

	// CARRY gets MSB from register
	fprintf(VHDL, "\t\t\t\tCARRY := %s(7);\n", op1);

	// Shift register left 1 bit and append 0 (SL0)
	fprintf(VHDL, "\t\t\t\t%s := %s(6 downto 0) & '0';\n", op1, op1);

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_SL1(FILE *VHDL, char *op1) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- SL1 %s\n", op1);

	// CARRY gets MSB from operand 1
	fprintf(VHDL, "\t\t\t\tCARRY := %s(7);\n", op1);

	// Shift register left 1 bit and append 1 (SL1)
	fprintf(VHDL, "\t\t\t\t%s := %s(6 downto 0) & '1';\n", op1, op1);

	// Set ZERO flag to 0 as SL1 on register never equals 0
	fprintf(VHDL, "\t\t\t\tZERO := '0';\n\n");
}

void ins_SLA(FILE *VHDL, char *op1) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- SLA %s\n", op1);

	// Set TMP = s0 & CARRY
	fprintf(VHDL, "\t\t\t\tTMP := %s & CARRY;\n", op1);

	// Set new register value
	fprintf(VHDL, "\t\t\t\t%s := TMP(7 downto 0);\n", op1);

	// Set new CARRY flag
	fprintf(VHDL, "\t\t\t\tCARRY := TMP(8);\n");

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_SLX(FILE *VHDL, char *op1) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- SLX %s\n", op1);

	// Set CARRY flag to MSB
	fprintf(VHDL, "\t\t\t\tCARRY := %s(7);\n", op1);

	// Save the register bits (6 downto 0) & register bit (0)
	fprintf(VHDL, "\t\t\t\t%s := %s(6 downto 0) & %s(0);\n", op1, op1, op1);

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_SR0(FILE *VHDL, char *op1) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- SR0 %s\n", op1);

	// CARRY gets LSB from register
	fprintf(VHDL, "\t\t\t\tCARRY := %s(0);\n", op1);

	// Shift register right 1 bit and append 0 to bit 8 (SR0)
	fprintf(VHDL, "\t\t\t\t%s := '0' & %s(7 downto 1);\n", op1, op1);

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_SR1(FILE *VHDL, char *op1) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- SR1 %s\n", op1);

	// CARRY gets LSB from register
	fprintf(VHDL, "\t\t\t\tCARRY := %s(0);\n", op1);

	// Shift register right 1 bit and append 0 to bit 8 (SR0)
	fprintf(VHDL, "\t\t\t\t%s := '1' & %s(7 downto 1);\n", op1, op1);

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_SRA(FILE *VHDL, char *op1) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- SRA %s\n", op1);

	// Set TMP = CARRY & s0
	fprintf(VHDL, "\t\t\t\tTMP := CARRY & %s;\n", op1);

	// Set new register value
	fprintf(VHDL, "\t\t\t\t%s := TMP(8 downto 1);\n", op1);

	// Set new CARRY flag
	fprintf(VHDL, "\t\t\t\tCARRY := TMP(0);\n");

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_SRX(FILE *VHDL, char *op1) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- SRX %s\n", op1);

	// Set CARRY flag to LSB
	fprintf(VHDL, "\t\t\t\tCARRY := %s(0);\n", op1);

	// Save the register MSB & register bits (7 downto 1)
	fprintf(VHDL, "\t\t\t\t%s := %s(7) & %s(7 downto 1);\n", op1, op1, op1);

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_RL(FILE *VHDL, char *op1) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- RL %s\n", op1);

	// Set CARRY flag to MSB
	fprintf(VHDL, "\t\t\t\tCARRY := %s(7);\n", op1);

	// Save the register bits (6 downto 0) & MSB
	fprintf(VHDL, "\t\t\t\t%s := %s(6 downto 0) & %s(7);\n", op1, op1, op1);

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_RR(FILE * VHDL, char *op1) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- RR %s\n", op1);

	// Set CARRY flag to LSB
	fprintf(VHDL, "\t\t\t\tCARRY := %s(0);\n", op1);

	// Save the register LSB & register bits (7 downto 1)
	fprintf(VHDL, "\t\t\t\t%s := %s(0) & %s(7 downto 1);\n", op1, op1, op1);

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_ADDCY(FILE *VHDL, char *op1, char *op2) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- ADDCY %s, %s\n", op1, op2);

	// If both operands are registers
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) + ('0' & %s) + (\"0000000\" & CARRY);\n", op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) + x\"%s\" + (\"0000000\" & CARRY);\n", op1, op2);
	}

	// Set the destination register
	fprintf(VHDL, "\t\t\t\t%s := TMP(7 downto 0);\n", op1);

	// Set the CARRY flag
	fprintf(VHDL, "\t\t\t\tCARRY := TMP(8);\n");

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") AND (ZERO = '1') then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_SUB(FILE *VHDL, char *op1, char *op2) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- SUB %s, %s\n", op1, op2);

	// If both operands are registers, perform SUB between two registers
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) - ('0' & %s);\n", op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) - x\"%s\";\n", op1, op2);
	}

	// Set the destination register
	fprintf(VHDL, "\t\t\t\t%s := TMP(7 downto 0);\n", op1);

	// Set the CARRY (BORROW) flag
	fprintf(VHDL, "\t\t\t\tCARRY := TMP(8);\n");

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_SUBCY(FILE *VHDL, char *op1, char *op2) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- SUBCY %s, %s\n", op1, op2);

	// If both operands are registers
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) - ('0' & %s) - (\"0000000\" & CARRY);\n", op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) - x\"%s\" - (\"0000000\" & CARRY);\n", op1, op2);
	}

	// Set the destination register
	fprintf(VHDL, "\t\t\t\t%s := TMP(7 downto 0);\n", op1);

	// Set the CARRY flag
	fprintf(VHDL, "\t\t\t\tCARRY := TMP(8);\n");

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") AND (ZERO = '1') then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_AND(FILE *VHDL, char *op1, char *op2) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- AND %s, %s\n", op1, op2);

	// If both operands are registers, perform AND between two registers
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\t%s := %s AND %s;\n", op1, op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\t%s := %s AND x\"%s\";\n", op1, op1, op2);
	}

	// Clear the CARRY flag
	fprintf(VHDL, "\t\t\t\tCARRY := '0';\n");

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_OR(FILE *VHDL, char *op1, char *op2) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- OR %s, %s\n", op1, op2);

	// If both operands are registers, perform AND between two registers
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\t%s := %s OR %s;\n", op1, op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\t%s := %s OR x\"%s\";\n", op1, op1, op2);
	}

	// Clear the CARRY flag
	fprintf(VHDL, "\t\t\t\tCARRY := '0';\n");

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_XOR(FILE *VHDL, char *op1, char *op2) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- XOR %s, %s\n", op1, op2);

	// If both operands are registers, perform AND between two registers
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\t%s := %s XOR %s;\n", op1, op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\t%s := %s XOR x\"%s\";\n", op1, op1, op2);
	}

	// Clear the CARRY flag
	fprintf(VHDL, "\t\t\t\tCARRY := '0';\n");

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_LOAD(FILE *VHDL, char *op1, char *op2) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- LOAD %s, %s\n", op1, op2);

	// LOAD operand 2 into register in operand 1
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\t%s := %s;\n\n", op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\t%s := x\"%s\";\n\n", op1, op2);
	}
}

void ins_COMPARE(FILE *VHDL, char *op1, char *op2) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- COMPARE %s, %s\n", op1, op2);

	// If both operands are registers, perform SUB between two registers storing in a TMP variable
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) - ('0' & %s);\n", op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) - x\"%s\";\n", op1, op2);
	}

	// Set the CARRY (BORROW) flag
	fprintf(VHDL, "\t\t\t\tCARRY := TMP(8);\n");

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_COMPARECY(FILE *VHDL, char *op1, char *op2) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- COMPARECY %s, %s\n", op1, op2);

	// If both operands are registers
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) - ('0' & %s) - (\"0000000\" & CARRY);\n", op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) - x\"%s\" - (\"0000000\" & CARRY);\n", op1, op2);
	}

	// Set the CARRY (BORROW) flag
	fprintf(VHDL, "\t\t\t\tCARRY := TMP(8);\n");

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n\n");
}

void ins_TEST(FILE *VHDL, char *op1, char *op2) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- TEST %s, %s\n", op1, op2);

	// If both operands are registers, perform AND between two registers
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) AND ('0' & %s);\n", op1, op2);
	} else {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) AND x\"%s\";\n", op1, op2);
	}

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\") then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n");

	// Set CARRY flag if parity is odd
	fprintf(VHDL, "\t\t\t\tCARRY := TMP(7) XOR TMP(6) XOR TMP(5) XOR TMP(4)"); 
	fprintf(VHDL, "XOR TMP(3) XOR TMP(2) XOR TMP(1) XOR TMP(0);\n\n");
}

void ins_TESTCY(FILE *VHDL, char *op1, char *op2) {
	// Instruction comment
	fprintf(VHDL, "\t\t\t\t-- TESTCY %s, %s\n", op1, op2);

	// If both operands are registers, perform AND between two registers
	if (strstr(op2, "s") != NULL) {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) AND ('0' & %s);\n", op1, op2);
	}
	else {
		fprintf(VHDL, "\t\t\t\tTMP := ('0' & %s) AND x\"%s\";\n", op1, op2);
	}

	// Set the ZERO flag
	fprintf(VHDL, "\t\t\t\tif (%s = \"00000000\" AND ZERO = '1') then\n", op1);
	fprintf(VHDL, "\t\t\t\t\tZERO := '1';\n\t\t\t\telse \n\t\t\t\t\tZERO := '0';\n\t\t\t\tend if;\n");

	// Set CARRY flag if parity is odd and CARRY = 1
	fprintf(VHDL, "\t\t\t\tTMP(8) := TMP(7) XOR TMP(6) XOR TMP(5) XOR TMP(4)");
	fprintf(VHDL, "XOR TMP(3) XOR TMP(2) XOR TMP(1) XOR TMP(0);\n");
	fprintf(VHDL, "\t\t\t\tCARRY := CARRY AND TMP(8);\n\n");
}