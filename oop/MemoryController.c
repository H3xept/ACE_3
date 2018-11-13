/**
* Filename: MemoryController.c
* Class: MemoryController
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-11
* 
* Description: Proxy level between memory and control unit
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "Object.h"
#include "OOP.h"
#include "../utilities/utilities.h"
#include "MemoryController.h"

#define WORD_SIZE 16
#define MIN_BITS_ADDRESSABLE 16
#define ADDR_LENGTH 12
#define WORDS_IN_MEM (int)(((pow(2,ADDR_LENGTH))*MIN_BITS_ADDRESSABLE)/WORD_SIZE)
#define TOTAL_MEM WORD_SIZE*WORDS_IN_MEM
// Arbitrary: we just assumed that the stack will be big enough being 1/4 of total memory.
#define MAX_PROG_SIZE ((3.0/4.0)*TOTAL_MEM) 

/// The type string of MemoryController
static const char* const 	type_string = "MemoryController";

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

/// Class descriptor structure to instantiate MemoryController
static struct Class_Descriptor _MemoryController_Class_Descriptor = {
	sizeof(MemoryController),
	&vtbl
};
const void * MemoryController_Class_Descriptor = &_MemoryController_Class_Descriptor;

// Private Fields
static const void* __memory;

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: MemoryController constructor.
* @param self: A reference to the current instance of MemoryController
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	// MemoryController* _self = (MemoryController*)self;
	__memory = calloc(1,TOTAL_MEM);
	return self;
}

/**
* @brief: MemoryController destructor.
* @param self: A reference to the current instance of MemoryController
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	// MemoryController* _self = (MemoryController*)self;
	if (__memory){
		free((void*)__memory); 
		__memory = 0;
	}
	return self;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of MemoryController
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of MemoryController
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	//MemoryController* _self = (MemoryController*)self;
	return "<MemoryController>";
}

static unsigned int _Object_Equals(Object* self, Object* obj)
{
	return Object_Equals(self,obj);
}

// Private class methods for MemoryController
// ...

// Private instance methods for MemoryController
static void* __Ptr_For_Address(MemoryController* self, int16_t addr)
{
	static int address_mask;
	if (!address_mask) 
		address_mask= pow(2, ADDR_LENGTH);
	return ((int16_t*)__memory) + (addr & address_mask);
}

static void __Set_Word_At_Ptr(MemoryController* self, int16_t* ptr, int16_t word)
{
	*ptr = word;
}

// Public class methods for MemoryController
// ...

// Public instance methods for MemoryController
int16_t MemoryController_Word_At_Address(MemoryController* self, int16_t addr)
{
	_info("Retrieving word at address %d (real addr: %p)", addr, __Ptr_For_Address(self, addr));
	return *((int16_t*)__Ptr_For_Address(self,addr));
}

void MemoryController_Set_Word_At_Address(MemoryController* self, int16_t addr, int16_t word)
{
	_info("Setting %d = %d (real addr: %p)",addr,word, __Ptr_For_Address(self,addr));
	__Set_Word_At_Ptr(self, __Ptr_For_Address(self, addr), word);
}

void MemoryController_Clear_Memory(MemoryController* self)
{
	_info("Clearing memory...",NULL);
	memset((void*)__memory, 0x0, TOTAL_MEM);
}

void MemoryController_Load_Memory_From_Ptr(MemoryController* self, void* ptr, size_t size)
{
	_info("Loading memory from %p | size: %lu", ptr, size);
	if (size >MAX_PROG_SIZE)
		_err("Trying to load a program bigger than the max allowed size. (Prog. size: %d | Max size: %d", size, MAX_PROG_SIZE);
	MemoryController_Clear_Memory(self);
	memcpy((void*)__memory, ptr, size);
}
