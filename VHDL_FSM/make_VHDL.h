#ifndef make_VHDL_h
#define make_VHDL_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int convert_int_to_bin(int decimal);
int num_digits(int n);
void case_State_Or_Wait(FILE *VHDL, int total_num_states, int state_num, char states[1000][10], char wait_states[1000][10], int wait);
void make_VHDL(FILE *VHDL, int num_states, int num_inputs, int num_outputs);
void make_Testbench(FILE *TB, int num_states, int num_inputs, int num_outputs);

#endif