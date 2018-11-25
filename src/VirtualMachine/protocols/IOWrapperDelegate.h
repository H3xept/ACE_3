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

#ifndef __IOWRAPPER_DELEGATE__
#define __IOWRAPPER_DELEGATE__

#include "../../oop/umbrella.h"
#include "../constants/var_word_size.h"

struct IOWrapperDelegate {
	void* delegateObject;
	uword_t* 	(*IOWrapperDelegate_Input)(struct IOWrapperDelegate * delegate);
	void 	(*IOWrapperDelegate_Output)(struct IOWrapperDelegate * delegate, Queue* output_queue);
};

#endif
