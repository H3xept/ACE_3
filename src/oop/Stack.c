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
* Filename: Stack.c
* Class: Stack
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-23
* 
* Description: A stack of pointers
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Object.h"
#include "OOP.h"
#include "../utilities/utilities.h"
#include "Stack.h"

/// The type string of Stack
static const char* const 	type_string = "Stack";

/// Private overrides for 'Object' virtual methods (signature)
static Object* 			 	_Object_Ctor(Object * self, va_list args);
static Object* 			 	_Object_Dtor(Object * self);
static const char* const 	_Object_Type_Descriptor(Object * _self);
static const char* const 	_Object_Descriptor(Object * self);
static unsigned int 		_Object_Equals(Object* self, Object* obj);

/// Function binding for virtual methods table
static struct ObjectVTbl 	vtbl = {
								&_Object_Ctor,
								&_Object_Dtor,
								&_Object_Type_Descriptor,
								&_Object_Descriptor,
								&_Object_Equals
							};

/// Class descriptor structure to instantiate Stack
static struct Class_Descriptor _Stack_Class_Descriptor = {
	sizeof(Stack),
	&vtbl
};
const void * Stack_Class_Descriptor = &_Stack_Class_Descriptor;

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: Stack constructor.
* @param self: A reference to the current instance of Stack
* @param args: Variadic args list as follows:
* - int: Number of elements for stack to hold.
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	// Downcast to Stack
	Stack* _self = (Stack*)self;
	size_t elements_n = va_arg(args, int);
	_self->__elements = calloc(elements_n, sizeof(void*));
	_self->max_len =  elements_n;
	_self->length = 0;
	return self;
}

/**
* @brief: Stack destructor.
* @param self: A reference to the current instance of Stack
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	// Downcast to Stack
	Stack* _self = (Stack*)self;
	if (_self->__elements)
		free(_self->__elements); _self->__elements = 0;
	return self;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of Stack
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of Stack
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	return "<Stack>";
}

/**
* @brief: Returns 0 if object is not the same instance as another.
* @param self: A reference to the current instance of Stack.
* @param obj: A reference to the another instance of Stack.
* @return: unsigned int: 0 if not equal.
*/
static unsigned int _Object_Equals(Object* self, Object* obj)
{
	return self == obj;
}


// Public instance methods for Stack

/**
* @brief: Pushes the given element onto the stack.
* @param self: A reference to the current instance of Stack.
* @param element: The element to be pushed onto the stack.
* @return: int - 1 if push was successful, 0 otherwise.
*/
int Stack_Push(Stack* self, void* element)
{
	if (self->length + 1 == self->max_len)
		return -1; // Stack Full - Stack smash detected!
	*(self->__elements+self->length++) = element;
	return 1;
}

/**
* @brief: Pops the given element from the stack.
* @param self: A reference to the current instance of Stack.
* @return: void* - Pointer to popped element.
*/
void* Stack_Pop(Stack* self)
{
	if (self->length >= self->max_len) 
	{
		_err("Stack max capacity reached. Aborting ...", NULL);
	}

	if (self->length == 0)
	{
		_warn("Stack has length 0. Returning NULL.", NULL);
		return NULL;
	}

	void* rt = *(self->__elements+(--(self->length)));
	*(self->__elements+self->length) = 0;
	return rt;
}

/**
* @brief: Examines the top element of the stack.
* @param self: A reference to the current instance of Stack.
* @return: void* - Pointer to examined element.
*/
void* Stack_Examine(Stack* self)
{
	if (self->length == 0){
		return NULL;
	}
	return *(self->__elements+self->length-1);
}

/**
* @brief: Returns if stack is empty.
* @param self: A reference to the current instance of Stack.
* @return: unsigned int - boolean representing if stack is empty.
*/
unsigned int Stack_Is_Empty(Stack* self)
{
	return self->length == 0;
}
