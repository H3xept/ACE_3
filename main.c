#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include "./oop/umbrella.h"
#include "VirtualMachine/Program.h"
#include "VirtualMachine/VirtualMachine.h"
#include <string.h>
#include <math.h>
#include "Disassembler/Disassembler.h"

#define k_HELP_STRING "Help:\n\
Usage: ... <disassembler_flag> <print_flag> <build_flag>\
disassembler_flag:\
	-s (or --static): Disassemble statically\n\
	-d (or --dynamic): Disassemble dynamically\n\
print_flag:\
	-p (or --print): Print contents of memory after execution\n\
build_flag:\
	-d (or --default): Load default program in memory\n\
	-c (or --console): Load a program from stdin in memory\n\
	-f (or --file) <filename> : Load a program from a file\n"

#define k_ONE_MORE_WORDS_OUT_BOUNDARIES "One or more words out of boundaries (MAX: %d)"
#define k_NOT_ENOUGH_PARAMS "Not enough arguments. \
Usage: %s <disassembler_flag> <print_flag> <build_flag>.\n\
Type 'help' for the list of available commands."

#define k_PRE_INPUT "Input accepted in decimal and hex:\n"

#define k_FLAG_DEFAULT "-d"
#define k_FLAG_DEFAULT_CAP "--default"

#define k_DEFAULT_PROGRAM_NAME "out3"

void display_help(void)
{
	_info(k_HELP_STRING,NULL);
}

uint16_t* input_words(void)
{
	uint16_t max_accepted = (uword_t)pow(2, 16)-1;

	uint16_t* ret = malloc(MAX_PROG_SIZE+1);
	char* buff = malloc(MAX_PROG_SIZE);
	int c = 0;

	memset(ret, '\0', MAX_PROG_SIZE);
	memset(buff, '\0', MAX_PROG_SIZE);

	printf(">>> ");

	fgets(buff, MAX_PROG_SIZE, stdin);
	char* last = NULL;
	long signed_word = strtol(buff, &last, 0);
	errno = 0;

	while(!errno && ++c)
	{
		if (signed_word > max_accepted){
			_warn(k_ONE_MORE_WORDS_OUT_BOUNDARIES, max_accepted);
			return input_words();
		}
		*(ret+c) = (uint16_t)signed_word;
		signed_word = strtol(last, &last, 0);
	} *(ret) = c;

	free(buff);

	return ret;
}

Program* handle_build_flag(const char* flag, int argc, char const *argv[])
{
	va_list args;
	Program* program = NULL;
	uint16_t* words = NULL;
	uint16_t n_words = 0;
	const char* filename;

	if (!strcmp(flag,k_FLAG_DEFAULT) || !strcmp(flag,k_FLAG_DEFAULT_CAP)) {

		program = Program_With_File(k_DEFAULT_PROGRAM_NAME);

	} else if (!strcmp(flag, "-c") || !strcmp(flag, "--console")) {

		printf(k_PRE_INPUT);
		words = input_words();
		n_words = *words;
		program = Program_With_Buffer(++words,n_words);

	} else if (!strcmp(flag, "-f") || !strcmp(flag, "--file")) {

		if (argc < 2) {
			_err("Filepath required. Usage ... %s <filepath>", flag);
		} filename = argv[0];
		program = Program_With_File(filename);

	} else if (!strcmp(flag, "help")) {
		
		display_help();
		exit(0);

	} else {
		return NULL;
	}

	return program;
}

int main(int argc, char const *argv[])
{
	// Program* program;
	// VirtualMachine* vm;

	// if (argc < 4)
	// {
	// 	_err(k_NOT_ENOUGH_PARAMS, argv[0]);
	// }

	// program = handle_build_flag(argv[1], argc, argv+2);
	// vm = alloc_init(VirtualMachine_Class_Descriptor);
	// Virtual_Machine_Run(vm, program);
	// return 0;
	Program* p = Program_With_File("out3");
	Disassembler* dsm = Disassembler_With_Program(p);
	Disassembler_Statically_Disassemble(dsm);
	return 0;
}
