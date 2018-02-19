#ifndef instruction_templates_h
#define instruction_templates_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

void ins_ADD(FILE *VHDL, char *op1, char *op2);
void ins_READ(FILE *VHDL, instruction ins, char *op1, char *op2, char *wait_state, char *next_state);
void ins_WRITE(FILE *VHDL, instruction ins, char *op1, char *op2, char *wait_state, char *next_state);
void ins_SL0(FILE *VHDL, char *op1);
void ins_SL1(FILE *VHDL, char *op1);
void ins_SLA(FILE *VHDL, char *op1);
void ins_SLX(FILE *VHDL, char *op1);
void ins_SR0(FILE *VHDL, char *op1);
void ins_SR1(FILE *VHDL, char *op1);
void ins_SRA(FILE *VHDL, char *op1);
void ins_SRX(FILE *VHDL, char *op1);
void ins_RL(FILE *VHDL, char *op1);
void ins_RR(FILE *VHDL, char *op1);
void ins_ADDCY(FILE *VHDL, char *op1, char *op2);
void ins_SUB(FILE *VHDL, char *op1, char *op2);
void ins_SUBCY(FILE *VHDL, char *op1, char *op2);
void ins_AND(FILE *VHDL, char *op1, char *op2);
void ins_OR(FILE *VHDL, char *op1, char *op2);
void ins_XOR(FILE *VHDL, char *op1, char *op2);
void ins_LOAD(FILE *VHDL, char *op1, char *op2);

#endif