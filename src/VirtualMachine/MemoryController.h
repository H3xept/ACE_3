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

#endif
