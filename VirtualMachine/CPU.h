/**
* Filename: CPU.h
* Class: CPU
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-16
* 
* Description: __DESCRIPTION__
*/

#ifndef __CPU__
#define __CPU__

#include "ALU.h"
#include "CU.h"
#include "./protocols/FlagDelegate.h"
#include "./protocols/MemoryDelegate.h" // Acting as an adaptor
#include "./protocols/IODelegate.h" // Acting as an adaptor
#include "MemoryController.h"
#include "VirtualMachine.h"
#include "IO.h"

struct CPU {
	Object super;
	VirtualMachine* vm;
	ALU* __alu;
	CU* __controlUnit;
	Registers* __registers;
	FlagRegister* __flagRegister;
	MemoryController* __memoryController;
	IO* __iOController;
	struct FlagDelegate* flagDelegateVptr;
	struct MemoryDelegate* memoryDelegateVptr;
	struct IODelegate* iODelegateVptr;
};

extern const void * CPU_Class_Descriptor;

// Public class methods for CPU
// ...

// Public instance methods for CPU
void CPU_Fetch_Execute_Cycle(CPU* self);
void CPU_Load_Words_In_Input_Queue(CPU* self, uword_t* words, size_t word_n);
#endif
