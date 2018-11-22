/**
* Filename: Program.c
* Class: Program
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-20
* 
* Description: A program for the ACE3 Machine
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../utilities/utilities.h"
#include "../oop/umbrella.h"
#include "Program.h"
#include <errno.h>

const int n = 1;
#define machine_is_bigendian() ( (*(char*)&n) == 0 )

/// The type string of Program
static const char* const 	type_string = "Program";

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

/// Class descriptor structure to instantiate Program
static struct Class_Descriptor _Program_Class_Descriptor = {
	sizeof(Program),
	&vtbl
};
const void * Program_Class_Descriptor = &_Program_Class_Descriptor;

// Private fields for Program
// ...

// Private class method declarations for Program
uword_t __To_Big_Endian(uword_t word);

// Private instance method declarations for Program
// ...

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: Program constructor.
* @param self: A reference to the current instance of Program
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	return self;
}

/**
* @brief: Program destructor.
* @param self: A reference to the current instance of Program
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	// Downcast to Program
	Program* _self = (Program*)self;
	if (_self->source)
		free(_self->source); _self->source = 0;
	return self;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of Program
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of Program
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	return "<Program>";
}

static unsigned int _Object_Equals(Object* self, Object* obj)
{
	// Downcast to Program
	Program* _self = (Program*)self;
	Program* obj_program;

	if (Object_Type_Descriptor(obj) != Object_Type_Descriptor(self))
		return 0;
	
	obj_program = (Program*)obj;

	if (obj_program->size != _self->size)
		return 0;

	for (int i = 0; i < _self->size; i++)
	{
		if (_self->source[i] != obj_program->source[i])
			return 0;
		return 1;
	}
	return 0;
}

// Private class methods for Program
uword_t __To_Big_Endian(uword_t word)
{	
	#if WORD_SIZE == 16
	return ((word << 8) | (word >> 8));
	#else 
	return ((word << 24) | ((word << 8) & 0x00ff0000) | ((word >> 8) & 0x0000ff00) | (word >> 24)); 
	#endif	
}

// Private instance methods for Program
// ...

// Public class methods for Program
Program* Program_With_File(const char* filename)
{
	FILE* file = fopen(filename,"r");
	uword_t word;
	Program* rt = alloc_init(Program_Class_Descriptor);
	uword_t* buffer = calloc(MAX_PROG_SIZE/WORD_SIZE, sizeof(uword_t));
	int c = 0;

	if (!file) {
		_err("Cannot find %s, aborting...", filename);
	}

	while(fread(&word, sizeof(uword_t), 1, file))
	{
		if (!machine_is_bigendian()){
			word = __To_Big_Endian(word);
		}
		*(buffer+c) = word;
		c++;
	}

	buffer = realloc(buffer, c*sizeof(uword_t));

	rt->source = buffer;
	rt->size = c;

	return rt;
}

Program* Program_With_Buffer(uword_t* buffer, size_t len)
{
	Program* rt = alloc_init(Program_Class_Descriptor);
	uword_t* new_buffer = calloc(len, sizeof(uword_t));

	rt->size = len;
	rt->source = new_buffer;
	for (int i = 0; i < len; i++)
		*(new_buffer+i) = *(buffer+i);

	return rt;
}


// Public instance methods for Program
// ...
