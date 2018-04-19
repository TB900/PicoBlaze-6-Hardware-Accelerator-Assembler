#include "parser.h"

// Parts of this code was adapted from here: https://www.lemoda.net/c/unix-regex/

// Error message size
#define ERROR_MSG 0x1000

// Corresponding table of instructions for instruction enumerables
char *instable[] = { "ADD", "STORE", "FETCH", "SL0", "INPUT", "OUTPUT", "SL1", "SLA", "SLX", "SR0", "SR1", "SRA", 
"SRX", "RL", "RR", "ADDCY", "SUB", "SUBCY", "AND", "OR", "XOR", "LOAD", "COMPARE", "COMPARECY", "TEST", "TESTCY", "LOOP", "END_LOOP", NULL };
 
//Operator ++ defintion for the instruction enumerables
instruction& operator++(instruction& p) {
	switch (p) {
		case (ADD): p = STORE; break;
		case (STORE): p = FETCH; break;
		case (FETCH): p = SL0; break;
		case (SL0): p = INPUT; break;
		case (INPUT): p = OUTPUT; break;
		case (OUTPUT): p = SL1; break;
		case (SL1): p = SLA; break;
		case(SLA): p = SLX; break;
		case(SLX): p = SR0; break;
		case(SR0): p = SR1; break;
		case(SR1): p = SRA; break;
		case(SRA): p = SRX; break;
		case(SRX): p = RL; break;
		case(RL): p = RR; break;
		case(RR): p = ADDCY; break;
		case(ADDCY): p = SUB; break;
		case(SUB): p = SUBCY; break;
		case(SUBCY): p = AND; break;
		case(AND): p = OR; break;
		case(OR): p = XOR; break;
		case(XOR): p = LOAD; break;
		case(LOAD): p = COMPARE; break;
		case(COMPARE): p = COMPARECY; break;
		case(COMPARECY): p = TEST; break;
		case(TEST): p = TESTCY; break;
		case(TESTCY): p = LOOP; break;
		case(LOOP): p = END_LOOP; break;
		case(END_LOOP): p = ADD; break;
	}
	return p;
}

/* Checks whether this inputted instruction string is contained within the instable 
If it is, return the corresponding instruction enumerable */
static instruction check_instruction(const char *ins, int line_num) {

	instruction checker = ADD;
	for (int i = 0; instable[i] != NULL; i++, ++checker) {
		if (strcmp(ins, instable[i]) == 0) {
			return checker;
		}
	}
	// If an invalid instruction is entered, print an error and exit the program 
	fprintf(stderr, "Error: instruction %s on line %i is invalid (instruction doesn't exist/unsupported)!\n", ins, line_num);
	exit(-1);
}

/* Checks the operands for a given instruction node in the dataflow struct has valid operands for the instruction
If they are valid, continue */
static int check_operands(dataflow **line, int line_num) {
	dataflow *check_line = *line;

	// Checks whether the operands are valid based on the instruction enumerable within the node.
	switch (check_line->ins) {
		case(ADD):
		case(STORE):
		case(FETCH):
		case(INPUT):
		case(OUTPUT):
		case(ADDCY):
		case(SUB):
		case(SUBCY):
		case(AND):
		case(OR):
		case(XOR):
		case(LOAD):
		case(COMPARE):
		case(COMPARECY):
		case(TEST):
		case(TESTCY):

		/* Case statement for all 2 operand instructions without conditionals:
		Check that the first operand is a valid register and not a conditional
		Check that the second operand isn't empty (will contain valid value due to the reguar expression if non-empty)
		Either of these checks fail, then a specific error is returned
		Otherwise the instruction is valid */
			if (strncmp(check_line->op1, "s", 1) != 0) {
				fprintf(stderr, "Error: Instruction %s on line %i has an invalid first operand!\n", instable[check_line->ins], line_num);
				exit(-1);
			} else if (strcmp(check_line->op2, "") == 0) {
				fprintf(stderr, "Error: Instruction %s on line %i has no second operand (or is not capitlised)!\n", instable[check_line->ins], line_num);
				exit(-1);
			} else {
				return 0;
			}

		case(SL0):
		case(SL1):
		case(SLA):
		case(SLX):
		case(SR0):
		case(SR1):
		case(SRA):
		case(SRX):
		case(RL):
		case(RR):

			/* Case statement for all 1 operand instructions without conditionals
			Checks whether the first operand is a valid register and not a conditional
			Check the second operand is empty
			Either of these checks fail, then a specific error is returned
			Otherwise the instruction is valid */
			if (strncmp(check_line->op1, "s", 1) != 0) {
				fprintf(stderr, "Error: Instruction %s on line %i has an invalid operand!\n", instable[check_line->ins], line_num);
				exit(-1);
			} else if (strcmp(check_line->op2, "") != 0) {
				fprintf(stderr, "Error: Instruction %s on line %i has an extra operand!\n", instable[check_line->ins], line_num);
				exit(-1);
			} else {
				return 0;
			}
		
		case(LOOP):
		case(END_LOOP):

			/* Case statement for all 1 operand conditional instructions 
			Checks whether the first operand is not a register (due to the regular expression, if doesn't start with an s, it must be a conditional)
			Checks whether the second operand is empty
			Either of these checks fail, then a specific error is returned
			Otherwise the instruction is valid */
			if (strncmp(check_line->op1, "s", 1) == 0) {
				fprintf(stderr, "Error: Instruction %s on line %i has a register for it's first operand!\n", instable[check_line->ins], line_num);
				exit(-1);
			} else if (strcmp(check_line->op2, "") != 0) {
				fprintf(stderr, "Error: Instruction %s on line %i has a second operand!\n", instable[check_line->ins], line_num);
				exit(-1);
			} else {
				return 0;
			}

		// The program should never reach this point but is left as a failsafe
		default:
			fprintf(stderr, "Error: Something has gone terribly wrong when checking the operands on the instruction on line %i!\n", line_num);
			exit(-1);
	}
}

// Prints the nodes of the dataflow linked list
void print_dataflow(dataflow **input) {
	dataflow *print = *input;
	print = print->next;

	while (print != NULL) {
		printf("Ins: %s; Op1: %s; Op2: %s; ST: %i;\n", instable[print->ins], print->op1, print->op2, print->state);
		print = print->next;
	}
}

// Matches the inputted assembly line to the inputted regular expression to parse the assembly into an instruction, operand 1 and operand 2
static int match_regex(regex_t *r, const char *to_match, dataflow **assembly, int line_num) {
	const char *prev_match = to_match; // Pointer to previous match
	const int n_matches = 5; // Maximum number of matches
	regmatch_t matches[n_matches]; // Contains matches from instruction
	dataflow *current = *assembly;
	static int state_num = 1; // Tracks the state the instrution appears in

	// Iterate to end of dataflow linked list
	while (current->next != NULL) {
		current = current->next;
	}

	// Allocate memory/values for the new node of the dataflow linked list
	current->next = (dataflow *)malloc(sizeof(dataflow)); // C++ syntaxing for Visual Studio
	current = current->next;
	current->ins = ADD;
	current->op1 = (char *)calloc(2, sizeof(char) * 2);
	current->op2 = (char *)calloc(2, sizeof(char) * 2);
	current->state = state_num;
	current->next = NULL;

	// An int which checks the line has been through the matching process
	int check_matching = 0;

	while (1) {
		int i = 0;
		int nomatch = regexec(r, prev_match, n_matches, matches, 0);

		// When no matches found with inputted string
		if (nomatch) {

			// Check whether any matches were found
			if (check_matching == 0) {
				fprintf(stderr, "Error: Syntax error on line %i (check the instruction is capitalised and the first operand is a condtional or register)!\n", line_num);
				exit(-1);
			}

			// Check whether operands are valid
			check_operands(&current, line_num);

			// If the instruction is a STORE or OUTPUT, increase the state number for next call
			if (current->ins == STORE || current->ins == OUTPUT) {
				state_num += 1;
			// Else if instruction is FETCH or INPUT, increased the state number and store it for the node
			} else if (current->ins == FETCH || current->ins == INPUT) {
				state_num += 1;
				current->state = state_num;
			}
			
			// Return from matching current instruction
			return nomatch;
		}

		// Iterate through the instruction for max number of matches and assign them to the node accordingly
		for (i = 0; i < n_matches; i++) {
			int start; // Pointer to start of capturing group sub-match within instruction
			int finish; // Pointer to end of capturing group sub-match within instruction

			// The line has been in the matching process
			check_matching = 1;

			if (matches[i].rm_so == -1) {
				break;
			}

			start = matches[i].rm_so + (prev_match - to_match);
			finish = matches[i].rm_eo + (prev_match - to_match);

			//Debug print statements
			if (i == 0) {
				printf("$& is ");
			} else {
				printf("$%d is ", i);
			}

			printf("'%.*s' (bytes %d:%d)\n", (finish - start), (to_match + start), start, finish);
			
			/* 
			If i is capturing group 1, assign it to the ins of the node 
			If i is capturing group 2, assign it to the op1 of the node 
			If i is capturing group 4, assign it to the op2 of the node
			*/
			if (i == 1) {
				char *instruction = (char *)calloc(15, sizeof(char) * 15);
				memcpy(instruction, (to_match + (start * 2)), (sizeof(char)*(finish - start)));
				current->ins = check_instruction(instruction, line_num);
			} else if (i == 2) {
				memcpy(current->op1, (to_match + start), (sizeof(char)*(finish - start)));
			} else if (i == 4) {
				memcpy(current->op2, (to_match + start), (sizeof(char)*(finish - start)));
			}
		}
		prev_match += matches[0].rm_eo;
	}
	return 0;
}

// The inputted filename is opened and the dataflow assembly linked list is created according to the instructions within the assembly file.
void parse_assembly (char *assembly_file, dataflow **assembly, int *num_ST, instruction *last_ins) {
	FILE *psm; // File pointer for the insputted filename
	int exists, comp; // Error checking ints
	regex_t reg_expr; // Where the regular expression string is compiled
	const char* assembly_reg_expr; // Regular expression string
	char instruction[25]; // Buffer for current assembly line from file

	// Check whether the file exists
	exists = fopen_s(&psm, assembly_file, "r");
	if (exists) {
		perror("Error");
		exit(-1);
	}
	
	// Regular expression for Picoblaze assembly is created and compiled
	// ([A-Z0-1]+)[ ]+([s][0-9|A-F])([, ])*([s][0-9|A-F]|[0-9A-F]{2})?
	assembly_reg_expr = "([A-Z_0-1]+)[ ]+([s][0-9|A-F]|[CZ]{1}|[N][CZ])([, ])*([s][0-9|A-F]|[0-9A-F]{2})?";
	comp = regcomp(&reg_expr, assembly_reg_expr, REG_EXTENDED); // | REG_NEWLINE

	// Checks whether the regular expression is invalid
	if (comp != 0) {
		char regex_error[ERROR_MSG];
		regerror(comp, &reg_expr, regex_error, ERROR_MSG);
		printf("Regex error compiling '%s': %s\n", assembly_reg_expr, regex_error);
		exit(-1);
	}

	// Iterate through each line of the assembly file parsing it into the dataflow linked list
	int line_num = 1; // Current line number in assembly file
	while (fgets(instruction, sizeof(instruction), psm) != NULL) {
		// Skips any line breaks in the file
		if (strcmp(instruction,"\n") != 0) {
			match_regex(&reg_expr, instruction, assembly, line_num);
		}
		line_num += 1; // Increment line number in assembly file
	}

	// Iterate to end of dataflow linked list to get the end node whilst getting the last mem instruction
	// Check that all loops have been closed
	dataflow *end_node = *assembly;
	int open_loop = 0;
	while (end_node->next != NULL) {
		end_node = end_node->next;

		// Increment the open loop count if it is a loop instruction
		if (end_node->ins == LOOP) {
			open_loop++;

		// Decrement the open loop count if it is an end loop instruction
		} else if (end_node->ins == END_LOOP) {
			open_loop--;

		// Store the instruction if it is a memory instruction
		} else if (end_node->ins == FETCH || end_node->ins == STORE || end_node->ins == INPUT || end_node->ins == OUTPUT) {
			
			// Check the memory instruction isn't contained within a loop
			if (open_loop == 0) {
				*last_ins = end_node->ins;
			} else {
				fprintf(stderr, "Error: A memory instruction (%s) is located within a loop! Memory instructions are currently not supported in loops!\n", instable[end_node->ins]);
				exit(-1);
			}
		}
	}

	// Check there are no open loops
	if (open_loop > 0) {
		fprintf(stderr, "Error: There is %i open loop/s within your code! Please check all loops are terminated!\n", open_loop);
		exit(-1);
	} else if (open_loop < 0) {
		fprintf(stderr, "Error: There is %i open loop/s within your code! Please check all loops are initialised!\n", abs(open_loop));
		exit(-1);
	}

	// Get the number of states required for the state machine from the final node
	*num_ST = end_node->state;

	// Free the memory for the regular expression
	regfree(&reg_expr);
}