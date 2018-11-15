/**
* Filename: MemoryController.h
* Class: MemoryController
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-11
* 
* Description: Proxy level between memory and control unit
*/

#ifndef __MemoryController__
#define __MemoryController__

#include "./protocols/MemoryDelegate.h"

typedef struct _MemoryController {
	Object super;
	struct MemoryDelegate* memoryDelegateVptr;
} MemoryController;

extern const void * MemoryController_Class_Descriptor;

// Public class methods for MemoryController
// ...

// Public instance methods for MemoryController
// ...

#endif
