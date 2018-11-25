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
* Filename: Stack.h
* Class: Stack
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-23
* 
* Description: A stack of pointers
*/

#ifndef __Stack__
#define __Stack__

#define Stack_Pop_Typed(STACK_SELF, TYPE) (TYPE)Stack_Pop(STACK_SELF)
#define Stack_Examine_Typed(STACK_SELF, TYPE) (TYPE)Stack_Examine(STACK_SELF)

typedef struct _Stack {
	Object super;
	void** __elements;
	size_t length;
	size_t max_len;
} Stack;

extern const void * Stack_Class_Descriptor;

// Public instance methods for Stack

int Stack_Push(Stack* self, void* element);
void* Stack_Pop(Stack* self);
void* Stack_Examine(Stack* self);
unsigned int Stack_Is_Empty(Stack* self);
#endif
