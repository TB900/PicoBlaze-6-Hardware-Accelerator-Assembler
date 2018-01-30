#include "parser.h"

// Reference: https://www.lemoda.net/c/unix-regex/
#define ERROR_MSG 0x1000
char *instable[] = { "ADD", "STORE", "FETCH", "SL0", NULL };

instruction& operator++(instruction& p) {
	switch (p) {
		case ADD: p = STORE; break;
		case STORE: p = FETCH; break;
		case FETCH: p = SL0; break;
		case SL0: p = ADD; break;
	}
	return p;
}

static instruction check_instruction(const char *ins, int line_num) {

	instruction checker = ADD;
	for (int i = 0; instable[i] != NULL; i++, ++checker) {
		if (strcmp(ins, instable[i]) == 0) {
			return checker;
		}
	}
	fprintf(stderr, "Error: instruction %s on line %i is invalid (instruction doesn't exist/unsupported)!\n", ins, line_num);
	exit(-1);
}

static int check_operands(dataflow **line, int line_num) {
	dataflow *check_line = *line;

	if (strcmp(check_line->op1, "") == 0) {
		fprintf(stderr, "Error: Instruction on line %i is invalid (error in operand 1)!\n", line_num);
		exit(-1);
	}

	switch (check_line->ins) {
		case(ADD):
		case(STORE):
		case(FETCH):
			if (strcmp(check_line->op2, "") != 0) {
				return 0;
			} else {
				fprintf(stderr, "Error: Instruction %s on line %i has no second operand!\n", instable[check_line->ins], line_num);
				exit(-1);
			}

		case(SL0):
			if (strcmp(check_line->op2, "") == 0) {
				return 0;
			} else {
				fprintf(stderr, "Error: Instruction %s on line %i has an extra operand!\n", instable[check_line->ins], line_num);
				exit(-1);
			}

		default:
			fprintf(stderr, "Error: Something has gone terribly wrong when checking the operands on the instruction on line %i!\n", line_num);
			exit(-1);
	}
}

static int match_regex(regex_t *r, const char *to_match, dataflow **assembly, int line_num) {
	const char *prev_match = to_match; // Pointer to previous match
	const int n_matches = 5; // Maximum number of matches
	regmatch_t matches[n_matches]; // Contains matches from instruction
	dataflow *current = *assembly;
	static int state_num = 0; // Tracks the state the instrution appears in

	while (current->next != NULL) {
		current = current->next;
	}

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

		if (nomatch) {
			// Check whether operands are valid
			check_operands(&current, line_num);

			// If the instruction is a STORE or FETCH, increase the state number for next call
			if (current->ins == 1 || current->ins == 2) {
				state_num += 1;
			}
			
			// Return from matching current instruction
			return nomatch;
		}

		for (i = 0; i < n_matches; i++) {
			int start;
			int finish;

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
			

			if (i == 1) {
				char *ins = (char *)calloc(10, sizeof(char) * 10);
				memcpy(ins, (to_match + (start * 2)), (sizeof(char)*(finish - start)));
				current->ins = check_instruction(ins, line_num);
				//printf("ins = %s\n", current->ins);
			} else if (i == 2) {
				memcpy(current->op1, (to_match + start), (sizeof(char)*(finish - start)));
				//printf("op1 = %s\n", current->op1);
			} else if (i == 4) {
				memcpy(current->op2, (to_match + start), (sizeof(char)*(finish - start)));
				//printf("op2 = %s\n", current->op2);
			} else {
				//fprintf(stderr, "Error: Too many operands!\n");
			}
		}
		prev_match += matches[0].rm_eo;
	}
	return 0;
}

int parse_assembly (char *assembly_file, dataflow **assembly) {
	FILE *psm;
	int exists, comp;
	regex_t reg_expr;
	const char* assembly_reg_expr;
	char instruction[25];

	exists = fopen_s(&psm, assembly_file, "r");
	if (exists) {
		perror("Error:");
		return -1;
	}
	
	assembly_reg_expr = "([A-Z0-1]+)[ ]+([s][0-9|A-F])([, ])*([s][0-9|A-F]|[0-9A-F]{2})?";
	comp = regcomp(&reg_expr, assembly_reg_expr, REG_EXTENDED | REG_NEWLINE);

	if (comp != 0) {
		char regex_error[ERROR_MSG];
		regerror(comp, &reg_expr, regex_error, ERROR_MSG);
		printf("Regex error compiling '%s': %s\n", assembly_reg_expr, regex_error);
		return 1;
	}

	int line_num = 1;
	while (fgets(instruction, sizeof(instruction), psm) != NULL) {
		match_regex(&reg_expr, instruction, assembly, line_num);
		line_num += 1;
	}

	dataflow *print = *assembly;
	print = print->next;
	
	while (print != NULL) {
		printf("Ins: %s; Op1: %s; Op2: %s; ST: %i;\n", instable[print->ins], print->op1, print->op2, print->state);
		print = print->next;
	}

	regfree(&reg_expr);
	return 0;
}