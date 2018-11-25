/**
* Filename: Stack.h
* Class: Stack
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-23
* 
* Description: A
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

// Public class methods for Stack
// ...

// Public instance methods for Stack

int Stack_Push(Stack* self, void* element);
void* Stack_Pop(Stack* self);
void* Stack_Examine(Stack* self);
unsigned int Stack_Is_Empty(Stack* self);
#endif
