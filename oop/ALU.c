/**
* Filename: ALU.c
* Class: ALU
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-12
* 
* Description: the place where arithmetic and logical operations take place
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Object.h"
#include "OOP.h"
#include "../utilities/utilities.h"
#include "ALU.h"

/// The type string of ALU
static const char* const 	type_string = "ALU";

/// Private overrides for 'Object' virtual methods (signature)
static Object* 			 	_Object_Ctor(Object * self, va_list args);
static Object* 			 	_Object_Dtor(Object * self);
static const char* const 	_Object_Type_Descriptor(Object * _self);
static const char* const 	_Object_Descriptor(Object * self);

/// Function binding for virtual methods table
static struct ObjectVTbl 	vtbl = {
								&_Object_Ctor,
								&_Object_Dtor,
								&_Object_Type_Descriptor,
								&_Object_Descriptor
							};

/// Class descriptor structure to instantiate ALU
static struct Class_Descriptor _ALU_Class_Descriptor = {
	sizeof(ALU),
	&vtbl
};
const void * ALU_Class_Descriptor = &_ALU_Class_Descriptor;

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: ALU constructor.
* @param self: A reference to the current instance of ALU
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	// Downcast to ALU
	ALU* _self = (ALU*)self;
	_warn("Class ALU does not respond to %s",__func__);
	assert(0);
	return NULL;
}

/**
* @brief: ALU destructor.
* @param self: A reference to the current instance of ALU
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	// Downcast to ALU
	ALU* _self = (ALU*)self;
	_warn("Class ALU does not respond to %s",__func__);
	assert(0);
	return NULL;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of ALU
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of ALU
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	// Downcast to ALU
	ALU* _self = (ALU*)self;
	_warn("Class ALU does not respond to %s",__func__);
	assert(0);
	return NULL;
}

// Private class methods for ALU
// ...

// Private instance methods for ALU
// ...

// Public class methods for ALU
// ...

// Public instance methods for ALU
// ...
