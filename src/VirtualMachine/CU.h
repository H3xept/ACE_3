/**************************************************************************
 * Assessment Title:
 * ACE 3 (TBTTBSQSA)
 *
 * Number of Submitted C Files: 
 * 19 (51 total files including .h, .py, .asm and .md)
 * 
 * Date: 
 * 25/11/2018
 * 
 * Authors: 
 *	1. Leonardo Cascianelli, Reg no: 201710666
 *	2. Rory Brown, Reg no: 201717976
 *	3. Ewan Skene, Reg no: 201717413
 * 
 *
 *	Statement: We confirm that this submission is all our own work.
 *
 *  (Signed)_Leonardo_Cascianelli________________(Leonardo Cascianelli)
 *	
 * 	(Signed)_Rory_Brown__________________________(Rory Brown)
 *	
 *	(Signed)_Ewan_Skene__________________________(Ewan Skene)
 *
 **************************************************************************/

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
