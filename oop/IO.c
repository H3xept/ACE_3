/**
* Filename: IO.c
* Class: IO
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-14
* 
* Description: __DESCRIPTION__
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Object.h"
#include "OOP.h"
#include "../utilities/utilities.h"
#include "IO.h"

/// The type string of IO
static const char* const 	type_string = "IO";

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

/// Class descriptor structure to instantiate IO
static struct Class_Descriptor _IO_Class_Descriptor = {
	sizeof(IO),
	&vtbl
};
const void * IO_Class_Descriptor = &_IO_Class_Descriptor;

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: IO constructor.
* @param self: A reference to the current instance of IO
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	// Downcast to IO
	IO* _self = (IO*)self;
	_warn("Class IO does not respond to %s",__func__);
	assert(0);
	return NULL;
}

/**
* @brief: IO destructor.
* @param self: A reference to the current instance of IO
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	// Downcast to IO
	IO* _self = (IO*)self;
	_warn("Class IO does not respond to %s",__func__);
	assert(0);
	return NULL;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of IO
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of IO
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	// Downcast to IO
	IO* _self = (IO*)self;
	_warn("Class IO does not respond to %s",__func__);
	assert(0);
	return NULL;
}

// Private class methods for IO
// ...

// Private instance methods for IO
// ...

// Public class methods for IO
// ...

// Public instance methods for IO
// ...
