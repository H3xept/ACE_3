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

#include "CPU.h"
#include "ALU.h"
#include "CU.h"
#include "./protocols/FlagDelegate.h"
#include "./protocols/MemoryDelegate.h" // Acting as an adaptor
#include "./protocols/IODelegate.h" // Acting as an adaptor
#include "MemoryController.h"
#include "IO.h"

typedef struct _CPU {
	Object super;
	ALU* __alu;
	CU* __controlUnit;
	Registers* __registers;
	Flag_Register* __flagRegister;
	MemoryController* __memoryController;
	IO* __iOController;
	struct FlagDelegate flagDelegateVptr;
	struct MemoryDelegate memoryDelegateVptr;
	struct IODelegate iODelegateVptr;
} CPU;

extern const void * CPU_Class_Descriptor;

// Public class methods for CPU
// ...

// Public instance methods for CPU
// ...

#endif
