/**
* Filename: IO.h
* Class: IO
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-14
* 
* Description: __DESCRIPTION__
*/

#ifndef __IO__
#define __IO__

#include "./protocols/IODelegate.h"

typedef struct _IO {
	Object super;
	struct IODelegate* iODelegateVptr;
} IO;

extern const void * IO_Class_Descriptor;

// Public class methods for IO
// ...

// Public instance methods for IO
// ...

#endif
