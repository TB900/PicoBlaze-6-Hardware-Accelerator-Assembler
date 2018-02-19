#include "parser.h"

// Parts of this code was adapted from here: https://www.lemoda.net/c/unix-regex/

// Error message size
#define ERROR_MSG 0x1000

// Corresponding table of instructions for instruction enumerables
char *instable[] = { "ADD", "STORE", "FETCH", "SL0", "INPUT", "OUTPUT", "SL1", "SLA", "SLX", "SR0", "SR1", "SRA", "SRX", "RL", "RR", "ADDCY", "SUB", "SUBCY", "AND", "OR", "XOR", "LOAD", NULL };
 
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
		case(LOAD): p = ADD; break;
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

	// Is the first operand empty? If so, return an error and exit
	if (strcmp(check_line->op1, "") == 0) {
		fprintf(stderr, "Error: Instruction on line %i is invalid (error in operand 1)!\n", line_num);
		exit(-1);
	}

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
			
			/* Case statement for all 2 operand instructions
			Already know the inputted operands are valid as they comply with the regular expression
			Checks whether the second operand is empty. If so, return an error and exit
			Else the operators are valid */
			if (strcmp(check_line->op2, "") != 0) {
				return 0;
			} else {
				fprintf(stderr, "Error: Instruction %s on line %i has no second operand!\n", instable[check_line->ins], line_num);
				exit(-1);
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

			/* Case statement for all 1 operand instructions 
			Checks whether the second operand is not empty, If so return an error and exit
			Else the operator is valid */
			if (strcmp(check_line->op2, "") == 0) {
				return 0;
			} else {
				fprintf(stderr, "Error: Instruction %s on line %i has an extra operand!\n", instable[check_line->ins], line_num);
				exit(-1);
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

	while (1) {
		int i = 0;
		int nomatch = regexec(r, prev_match, n_matches, matches, 0);

		// When no matches found with inputted string
		if (nomatch) {
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
				char *instruction = (char *)calloc(10, sizeof(char) * 10);
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
	assembly_reg_expr = "([A-Z0-1]+)[ ]+([s][0-9|A-F])([, ])*([s][0-9|A-F]|[0-9A-F]{2})?";
	comp = regcomp(&reg_expr, assembly_reg_expr, REG_EXTENDED | REG_NEWLINE);

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
	dataflow *end_node = *assembly;
	while (end_node->next != NULL) {
		end_node = end_node->next;

		// Store the instruction if it is a memory instruction
		if (end_node->ins == FETCH || end_node->ins == STORE || end_node->ins == INPUT || end_node->ins == OUTPUT) {
			*last_ins = end_node->ins;
		}
	}

	// Get the number of states required for the state machine from the final node
	*num_ST = end_node->state;

	// Free the memory for the regular expression
	regfree(&reg_expr);
}