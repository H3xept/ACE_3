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

#include <assert.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include "Object.h"
#include "Class_Descriptor.h"
#include "../utilities/utilities.h"

static const char* const 	type_string = "Object";

// INTERFACE - VIRTUAL METHODS
static Object* 			 	_Object_Ctor(Object * self, va_list args);
static Object* 			 	_Object_Dtor(Object * self);
static const char* const 	_Object_Type_Descriptor(Object * _self);
static const char* const 	_Object_Descriptor(Object * self);
static unsigned int 		_Object_Equals(Object* self, Object* obj);
// --

static struct ObjectVTbl 		vtbl = {
								&_Object_Ctor,
								&_Object_Dtor,
								&_Object_Type_Descriptor,
								&_Object_Descriptor,
								&_Object_Equals
							};

static struct Class_Descriptor _Object_Class_Descriptor = {
	sizeof(Object),
	&vtbl
};
const void * Object_Class_Descriptor = &_Object_Class_Descriptor;

// VIRTUAL METHODS (PRIVATE)


/**
* @brief: Object constructor.
* @param self: A reference to the current instance of Object
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	return self;
}

/**
* @brief: Object destructor.
* @param self: A reference to the current instance of Object
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	return self;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of Object
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of Object
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	return "<Object>";
}

/**
* @brief: Returns 0 if object is not the same instance as another.
* @param self: A reference to the current instance of object
* @param obj: A reference to the another instance of object
* @return: unsigned int: 0 if not equal.
*/
static unsigned int _Object_Equals(Object* self, Object* obj)
{
	return self == obj;
}

// VIRTUAL METHODS HANDLES (PUBLIC)
/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of Object
* @return: const char* const - The string that identifies the class.
*/
const char* const Object_Type_Descriptor(Object * self)
{
	assert(self);
	if (self->vptr->type_descriptor)
		return self->vptr->type_descriptor(self);
	_err("Type descriptor not found in vtable.", NULL);
	return NULL;
}


/**
* @brief: Object constructor.
* @param self: A reference to the current instance of Object
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
Object* Object_Ctor(Object * self, va_list args)
{
	assert(self);
	if (self->vptr->ctor)
		return self->vptr->ctor(self, args);
	_err("Object constructor not found in vtable.", NULL);
	return NULL;
}

/**
* @brief: Object destructor.
* @param self: A reference to the current instance of Object
* @return: Object* - The object to be freed.
*/
Object* Object_Dtor(Object * self)
{
	assert(self);
	if (self->vptr->dtor)
		return self->vptr->dtor(self);
	_err("Object distructor not found in vtable.", NULL);
	return NULL;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of Object
* @return: const char* const - The string that identifies the class.
*/
const char* const Object_Descriptor(Object * self)
{
	assert(self);
	if (self->vptr->descriptor)
		return self->vptr->descriptor(self);
	_err("Object descriptor not found in vtable.", NULL);
	return NULL;
}

/**
* @brief: Returns 0 if object is not the same instance as another.
* @param self: A reference to the current instance of object
* @param obj: A reference to the another instance of object
* @return: unsigned int: 0 if not equal.
*/
unsigned int Object_Equals(Object* self, Object* obj)
{
	assert(self && obj);
	if (self->vptr->equals)
		return self->vptr->equals(self,obj);
	_err("Object equality comparator not found in vtable", NULL);
	return 0;
}

// NON-VIRTUAL METHODS
