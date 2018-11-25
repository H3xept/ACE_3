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
