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
#include "../oop/umbrella.h"
#include "MemoryController.h"
#include "./protocols/MemoryDelegate.h"
#include "./constants/arch_const.h"

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

// Private fields for MemoryController
const void* __memory;

// Private class method declarations for MemoryController
// ...

// Private instance method declarations for MemoryController
static void __Setup_Delegates(MemoryController* self);

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
	MemoryController* _self = (MemoryController*)self;
	__memory = calloc(1,TOTAL_MEM);
	__Setup_Delegates(_self);
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

/**
* @brief: Returns 0 if object is not the same instance as another.
* @param self: A reference to the current instance of Memorycontroller
* @param obj: A reference to the another instance of Memorycontroller
* @return: unsigned int: 0 if not equal.
*/
static unsigned int _Object_Equals(Object* self, Object* obj)
{
	return Object_Equals(self,obj);
}

// Private class methods for MemoryController
// ...

// Private instance methods for MemoryController
/**
* @brief: gets a pointer for a value at the specified address.
* @param self: A reference to the current instance of Memorycontroller
* @param addr: int16/32_t containing address
* @return: pointer to the word at the address
*/
static void* __Ptr_For_Address(MemoryController* self, uword_t addr)
{
	static unsigned int address_mask;
	if (!address_mask) 
		address_mask = pow(2, ADDR_LENGTH)-1;
	return ((uword_t*)__memory) + (addr & address_mask);
}

/**
* @brief: sets word value at address of ptr.
* @param self: A reference to the current instance of Memorycontroller
* @param ptr: location to set word
* @param word: value to place at address ptr
*/
static void __Set_Word_At_Ptr(MemoryController* self, uword_t* ptr, uword_t word)
{
	*ptr = word;
}

// Public class methods for MemoryController
// ...

// Public instance methods for MemoryController
/**
* @brief: gets word at the specified address.
* @param self: A reference to the current instance of Memorycontroller
* @param addr: int16/32_t containing address
* @return: value stored at the address
*/
static uword_t MemoryDelegate_Word_At_Address(struct MemoryDelegate* delegate, uword_t addr)
{	
	MemoryController* _self = (MemoryController*)delegate->delegateObject; // Explicit downcast
	_info("Retrieving word at address %d (real addr: %p)", addr, __Ptr_For_Address(_self, addr));
	return *((uword_t*)__Ptr_For_Address(_self,addr));
}

/**
* @brief: sets word at the specified address.
* @param self: A reference to the current instance of Memorycontroller
* @param addr: int16/32_t containing address
*/
static void MemoryDelegate_Set_Word_At_Address(struct MemoryDelegate* delegate, uword_t addr, uword_t word)
{	
	MemoryController* _self = (MemoryController*)delegate->delegateObject; // Explicit downcast
	_info("Setting %d = %d (real addr: %p)",addr,word, __Ptr_For_Address(_self,addr));
	__Set_Word_At_Ptr(_self, __Ptr_For_Address(_self, addr), word);
}

/**
* @brief: sets all memory values to 0.
* @param self: A reference to the current instance of Memorycontroller
*/
static void MemoryDelegate_Clear_Memory(struct MemoryDelegate* delegate)
{	
	_info("Clearing memory...",NULL);
	memset((void*)__memory, 0x0, TOTAL_MEM);
}

/**
* @brief: loads memory from location ptr.
* @param self: A reference to the current instance of Memorycontroller
* @param ptr: location to get words from
* @param size: number of words to be loaded to memory
*/
static void MemoryDelegate_Load_Memory_From_Ptr(struct MemoryDelegate* delegate, void* ptr, size_t size)
{	
	_info("Loading memory from %p | size: %lu", ptr, size);
	if (size > MAX_PROG_SIZE)
		_err("Trying to load a program bigger than the max allowed size. (Prog. size: %d | Max size: %d", size, MAX_PROG_SIZE);
	MemoryDelegate_Clear_Memory(delegate);
	memcpy((void*)__memory, ptr, size);
}

static void __Setup_Delegates(MemoryController* self)
{
	_info("Setting up delegates for %s", __FILE__);
	
	static struct MemoryDelegate memoryDelegateVtbl = {
		0,
		&MemoryDelegate_Word_At_Address,
		&MemoryDelegate_Set_Word_At_Address,
		&MemoryDelegate_Clear_Memory,
		&MemoryDelegate_Load_Memory_From_Ptr
	};
	memoryDelegateVtbl.delegateObject = self;
	self->memoryDelegateVptr = &memoryDelegateVtbl;
}
