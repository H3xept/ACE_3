/**
* Filename: Disassembler.h
* Class: Disassembler
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-22
* 
* Description: A
*/

#ifndef __Disassembler__
#define __Disassembler__
#include "../VirtualMachine/Program.h"
#include "../VirtualMachine/constants/var_word_size.h"
#define MAX_LABELS 50

typedef struct {
	uword_t branch_address;
	uword_t ra_stack_length;
} SKC_Pair;

typedef struct {
	char label;
	int label_address;
} Label_Pair;

typedef struct _Disassembler {
	Object super;
	Program* program;
	uint8_t is_done_disassembling;
	char __current_label;
	Stack* __skc_stack;
	Stack* __ra_stack;
	Stack* __stored_ra_stack;
	uword_t __labels_n;
	Label_Pair* __label_list;
} Disassembler;

extern const void * Disassembler_Class_Descriptor;

// Public class methods for Disassembler
Disassembler* Disassembler_With_Program(Program* program);
// Public instance methods for Disassembler
char** Disassembler_Statically_Disassemble(Disassembler* self, int* size);

#endif
