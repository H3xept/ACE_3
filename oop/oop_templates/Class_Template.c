/**
* Filename: __CLASS_NAME__.c
* Class: __CLASS_NAME__
* Authors: __AUTHORS__
* Date: __DATE__
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
#include "__CLASS_NAME__.h"

/// The type string of __CLASS_NAME__
static const char* const 	type_string = "__CLASS_NAME__";

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

/// Class descriptor structure to instantiate __CLASS_NAME__
static struct Class_Descriptor ___CLASS_NAME___Class_Descriptor = {
	sizeof(__CLASS_NAME__),
	&vtbl
};
const void * __CLASS_NAME___Class_Descriptor = &___CLASS_NAME___Class_Descriptor;

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: __CLASS_NAME__ constructor.
* @param self: A reference to the current instance of __CLASS_NAME__
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	// Downcast to __CLASS_NAME__
	__CLASS_NAME__* _self = (__CLASS_NAME__*)self;
	_warn("Class __CLASS_NAME__ does not respond to %s",__func__);
	assert(0);
	return NULL;
}

/**
* @brief: __CLASS_NAME__ destructor.
* @param self: A reference to the current instance of __CLASS_NAME__
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	// Downcast to __CLASS_NAME__
	__CLASS_NAME__* _self = (__CLASS_NAME__*)self;
	_warn("Class __CLASS_NAME__ does not respond to %s",__func__);
	assert(0);
	return NULL;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of __CLASS_NAME__
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of __CLASS_NAME__
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	// Downcast to __CLASS_NAME__
	__CLASS_NAME__* _self = (__CLASS_NAME__*)self;
	_warn("Class __CLASS_NAME__ does not respond to %s",__func__);
	assert(0);
	return NULL;
}

// Private class methods for __CLASS_NAME__
// ...

// Private instance methods for __CLASS_NAME__
// ...

// Public class methods for __CLASS_NAME__
// ...

// Public instance methods for __CLASS_NAME__
// ...
