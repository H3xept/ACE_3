/**
* Filename: IO.h
* Class: IO
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-14
* 
* Description: Input Output Controller
*/

#ifndef __IO__
#define __IO__

#include "./protocols/IODelegate.h"
#include "../oop/umbrella.h"

typedef struct _IO {
	Object super;
	Queue* __in_q;
	Queue* __out_q;
	struct IODelegate* iODelegateVptr;
	struct FlagDelegate* __flagDelegate;
	struct IOWrapperDelegate* __iOWrapperDelegate;
} IO;

extern const void * IO_Class_Descriptor;

// Public class methods for IO
// ...

// Public instance methods for IO
// ...

#endif
