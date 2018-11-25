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
* Filename: OOP.c
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-16
* 
* Description: OOP initialiser and deallocation.
*/


#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "OOP.h"
#include "Object.h"
#include "../utilities/utilities.h"

/**
* @brief: initialises an object.
* @param class_descriptor: name to identify class
* @param ...: other initialisation parameters (optional/class dependant)
* @return: pointer to initialised object.
*/
void * alloc_init(const struct Class_Descriptor* descriptor, ...)
{
	assert(descriptor);
	va_list args;
	va_start(args, descriptor);

	void * ret = calloc(1,descriptor->size);

	((Object*)ret)->vptr = ((struct Class_Descriptor*)Object_Class_Descriptor)->vptr;
	Object_Ctor((Object*)ret, args);
	((Object*)ret)->vptr = descriptor->vptr;
	Object_Ctor((Object*)ret, args);
	va_end(args);

	return ret;
}

/**
* @brief: frees the memory holding an instance of an object
* @param ptr: pointer to object instance
*/
void dealloc(void * ptr)
{
	assert(ptr);
	Object* _self = (Object*)ptr;
	assert(_self);
	if (Object_Dtor(_self))
		free(_self);
	_warn("Object %s doesn't want to be deallocated.",Object_Descriptor(_self));
}
