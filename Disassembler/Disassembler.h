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
typedef struct _Disassembler {
	Object super;
	Program* program;
} Disassembler;

extern const void * Disassembler_Class_Descriptor;

// Public class methods for Disassembler
Disassembler* Disassembler_With_Program(Program* program);
// Public instance methods for Disassembler
char** Disassembler_Statically_Disassemble(Disassembler* self);

#endif
