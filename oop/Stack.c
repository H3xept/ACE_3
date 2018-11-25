/**
* Filename: Stack.c
* Class: Stack
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-23
* 
* Description: A
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

// Private fields for Stack
// ...

// Private class method declarations for Stack
// ...

// Private instance method declarations for Stack
// ...

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: Stack constructor.
* @param self: A reference to the current instance of Stack
* @param args: Variadic args list as follows:
* - type: desc
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

static unsigned int _Object_Equals(Object* self, Object* obj)
{
	return self == obj;
}

// Private class methods for Stack
// ...

// Private instance methods for Stack
// ...

// Public class methods for Stack
// ...

// Public instance methods for Stack
int Stack_Push(Stack* self, void* element)
{
	if (self->length + 1 == self->max_len)
		return -1; // Stack Full - Stack smash detected!
	*(self->__elements+self->length++) = element;
	return 1;
}

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

void* Stack_Examine(Stack* self)
{
	if (self->length == 0){
		return NULL;
	}
	return *(self->__elements+self->length-1);
}

unsigned int Stack_Is_Empty(Stack* self)
{
	return self->length == 0;
}
