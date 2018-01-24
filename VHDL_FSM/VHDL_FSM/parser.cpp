#include "parser.h"

// Reference: https://www.lemoda.net/c/unix-regex/
#define ERROR_MSG 0x1000

static int check_syntax(const char *ins, const char *op1, const char *op2) {
	switch () {

	}

	return 0;
}

static int match_regex(regex_t *r, const char *to_match, dataflow **assembly) {
	const char *prev_match = to_match; // Pointer to previous match
	const int n_matches = 4; // Maximum number of matches
	regmatch_t matches[n_matches]; // Contains matches from instruction
	dataflow *current = *assembly;

	while (current->next != NULL) {
		current = current->next;
	}

	current->next = (dataflow *)malloc(sizeof(dataflow)); // C++ syntaxing for Visual Studio
	current = current->next;
	current->ins = (char *)calloc(10, sizeof(char) * 10);
	current->op1 = (char *)calloc(2, sizeof(char) * 2);
	current->op2 = (char *)calloc(2, sizeof(char) * 2);
	current->next = NULL;

	while (1) {
		int i = 0;
		int nomatch = regexec(r, prev_match, n_matches, matches, 0);

		if (nomatch) {
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

			/*//Debug print statements
			if (i == 0) {
				printf("$& is ");
			} else {
				printf("$%d is ", i);
			}

			printf("'%.*s' (bytes %d:%d)\n", (finish - start), (to_match + start), start, finish);
			*/

			if (i == 1) {
				memcpy(current->ins, (to_match + (start * 2)), (sizeof(char)*(finish - start)));
				//printf("ins = %s\n", current->ins);
			} else if (i == 2) {
				memcpy(current->op1, (to_match + start), (sizeof(char)*(finish - start)));
				//printf("op1 = %s\n", current->op1);
			} else if (i == 3) {
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

	assembly_reg_expr = "([A-Z]+)[ ]+([s][0-9|A-F])[, ]+([s][0-9|A-F]|[0-9A-F]{2})";
	comp = regcomp(&reg_expr, assembly_reg_expr, REG_EXTENDED);

	if (comp != 0) {
		char regex_error[ERROR_MSG];
		regerror(comp, &reg_expr, regex_error, ERROR_MSG);
		printf("Regex error compiling '%s': %s\n", assembly_reg_expr, regex_error);
		return 1;
	}

	while (fgets(instruction, sizeof(instruction), psm) != NULL) {
		match_regex(&reg_expr, instruction, assembly);
	}

	dataflow *print = *assembly;
	print = print->next;

	while (print != NULL) {
		printf("Ins: %s, Op1: %s, Op2: %s\n", print->ins, print->op1, print->op2);
		print = print->next;
	}
	
	regfree(&reg_expr);
	return 0;
}