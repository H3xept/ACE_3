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
* Filename: VirtualMachine.h
* Class: VirtualMachine
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-20
* 
* Description: The encapsulating virtual machine object
*/

#ifndef __VirtualMachine__
#define __VirtualMachine__


#include "./protocols/IOWrapperDelegate.h"
#include "Program.h"
#include "VMLogger.h"

// Forward declaration - Actual structure in CPU.h
typedef struct CPU CPU;

typedef enum {
	CPU_Mode_Idle,
	CPU_Mode_Booting,
	CPU_Mode_Running
}CPU_Mode;

typedef struct _VirtualMachine {
	Object super;
	CPU* cpu;
	CPU_Mode cpu_mode;
	VMLogger* __logger;
	// Implements IOWrapperDelegate
	struct IOWrapperDelegate* iOWrapperDelegateVptr;
} VirtualMachine;

extern const void * VirtualMachine_Class_Descriptor;

// Public class methods for VirtualMachine
// ...

// Public instance methods for VirtualMachine
void Virtual_Machine_Run(VirtualMachine* self, Program* program);

#endif
