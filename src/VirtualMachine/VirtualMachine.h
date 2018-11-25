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
	// Implements IOWrapperDelegate
	struct IOWrapperDelegate* iOWrapperDelegateVptr;
} VirtualMachine;

extern const void * VirtualMachine_Class_Descriptor;

// Public class methods for VirtualMachine
// ...

// Public instance methods for VirtualMachine
void Virtual_Machine_Run(VirtualMachine* self, Program* program);

#endif
