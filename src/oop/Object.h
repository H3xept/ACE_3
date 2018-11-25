/**
* Filename: Object.h
* Class: Object
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-16
* 
* Description: Object superclass
*/

#ifndef __OBJECT__
#define __OBJECT__

#include <stdarg.h>

struct ObjectVTbl;
typedef struct _Object {
	const struct ObjectVTbl* vptr; // Virtual table pointer
} Object;

// NON_VIRTUAL METHODS

// VIRTUAL METHODS - HANDLES
Object* 				Object_Ctor(Object * self, va_list args);
Object* 				Object_Dtor(Object * self);
const char* const 		Object_Type_Descriptor(Object * self);
const char* const 		Object_Descriptor(Object * self);
unsigned int 			Object_Equals(Object* self, Object* obj);

// VIRTUAL METHODS
struct ObjectVTbl {
	Object* 			(*ctor)(Object* self, va_list args);
	Object* 			(*dtor)(Object* self);
	const char* const 	(*type_descriptor)(Object* self);
	const char* const 	(*descriptor)(Object* self);
	unsigned int		(*equals)(Object* self, Object* obj);
};

extern const void * Object_Class_Descriptor;

#endif
