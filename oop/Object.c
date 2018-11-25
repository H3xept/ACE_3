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

static Object* _Object_Ctor(Object * self, va_list args)
{
	return self;
}

static Object* _Object_Dtor(Object * self)
{
	return self;
}

static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

static const char* const _Object_Descriptor(Object * self)
{
	return "<Object>";
}

static unsigned int _Object_Equals(Object* self, Object* obj)
{
	return self == obj;
}

// VIRTUAL METHODS HANDLES (PUBLIC)

const char* const Object_Type_Descriptor(Object * self)
{
	assert(self);
	if (self->vptr->type_descriptor)
		return self->vptr->type_descriptor(self);
	_err("Type descriptor not found in vtable.", NULL);
	return NULL;
}

Object* Object_Ctor(Object * self, va_list args)
{
	assert(self);
	if (self->vptr->ctor)
		return self->vptr->ctor(self, args);
	_err("Object constructor not found in vtable.", NULL);
	return NULL;
}

Object* Object_Dtor(Object * self)
{
	assert(self);
	if (self->vptr->dtor)
		return self->vptr->dtor(self);
	_err("Object distructor not found in vtable.", NULL);
	return NULL;
}

const char* const Object_Descriptor(Object * self)
{
	assert(self);
	if (self->vptr->descriptor)
		return self->vptr->descriptor(self);
	_err("Object descriptor not found in vtable.", NULL);
	return NULL;
}

unsigned int Object_Equals(Object* self, Object* obj)
{
	assert(self && obj);
	if (self->vptr->equals)
		return self->vptr->equals(self,obj);
	_err("Object equality comparator not found in vtable", NULL);
	return 0;
}

// NON-VIRTUAL METHODS
