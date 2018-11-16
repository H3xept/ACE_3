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

typedef struct _CU {
	Object super;
	const Registers * __registers;
	const ALU * __alu;
	const IODelegate * __ioDelegate;
	const FlagDelegate * __flagDelegate;
	const MemoryDelegate * __memoryDelegate;
} CU;

extern const void * CU_Class_Descriptor;

// Public class methods for CU
// ...

// Public instance methods for CU
void CU_Execute_Instruction(CU* self, instruction_t instruction);

#endif
