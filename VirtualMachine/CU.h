/**
* Filename: CU.h
* Class: CU
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-16
* 
* Description: The control unit of the architecture
*/

#ifndef __CU__
#define __CU__

#include "./constants/registers.h"
#include "ALU.h"
#include "./protocols/IODelegate.h"
#include "./protocols/FlagDelegate.h"
#include "./protocols/MemoryDelegate.h"

typedef struct {
	uword_t opcode : 4;
	uword_t operand : 12;
} instruction_t;

typedef struct _CU {
	Object super;
	Registers * __registers;
	ALU * __alu;
	struct IODelegate * __ioDelegate;
	struct FlagDelegate * __flagDelegate;
	struct MemoryDelegate * __memoryDelegate;
} CU;

extern const void * CU_Class_Descriptor;

// Public class methods for CU
// ...

// Public instance methods for CU
void CU_Execute_Instruction(CU* self, instruction_t instruction);

#endif
