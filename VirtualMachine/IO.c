/**
* Filename: IO.c
* Class: IO
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-14
* 
* Description: Input Output Controller
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../oop/umbrella.h"
#include "./protocols/FlagDelegate.h"
#include "./protocols/IODelegate.h"
#include "./protocols/IOWrapperDelegate.h"
#include "./constants/var_word_size.h"
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

// Private fields for IO
// ...

// Private class method declarations for IO
// ...

// Private instance method declarations for IO
static void __Setup_Delegates(IO* self);

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
	_self->__flagDelegate = va_arg(args, struct FlagDelegate*);
	_self->__iOWrapperDelegate = va_arg(args, struct IOWrapperDelegate*);

	__Setup_Delegates(_self);

	_self->__in_q = alloc_init(Queue_Class_Descriptor);
	_self->__out_q = alloc_init(Queue_Class_Descriptor);
	return self;
}

/**
* @brief: IO destructor.
* @param self: A reference to the current instance of IO
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	IO* _self = (IO*)self;
	if (_self->__in_q)
		free(_self->__in_q); _self->__in_q = 0;
	if (_self->__out_q)
		free(_self->__out_q); _self->__out_q = 0;

	return self;
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
	return "<IO>";
}

// Private class methods for IO
// ...

// Private instance methods for IO
/**
* @brief: Returns a word from the input queue.
* @param IODelegate: A reference to the implementer of this method
* @return: uword_t - first word from the input queue.
*/
static uword_t IODelegate_Get_Word_From_Input_Queue(struct IODelegate * delegate)
{
	_delegCall();
	IO* self = (IO*)delegate->delegateObject;
	struct IOWrapperDelegate* iOWrapperDelegate = self->__iOWrapperDelegate;

	if(Queue_Is_Empty(self->__in_q)){
		uword_t* new_elements = iOWrapperDelegate->IOWrapperDelegate_Input(iOWrapperDelegate);
		uword_t n_elements = *new_elements; // 0th element contains the lenght of the list
		for (int n = 1; n < n_elements+1; n++){
			Queue_Enqueue(self->__in_q,*(new_elements+n));
			self->__flagDelegate->FlagDelegate_Set_Flag(self->__flagDelegate,k_Status_Flag_Input,1);
		}
	}
	word_t rtn = Queue_Dequeue(self->__in_q);
	self->__flagDelegate->FlagDelegate_Set_Flag(self->__flagDelegate,k_Status_Flag_Input,!Queue_Is_Empty(self->__in_q));
	return rtn;
}

/**
* @brief: puts a word onto the input queue.
* @param IODelegate: A reference to the implementer of this method
* @param word: value to add to the rear of the queue
* @param print: if 1 print to screen
*/
static void IODelegate_Put_Word_To_Output_Queue(struct IODelegate * delegate, uword_t word, uint8_t print)
{
	_delegCall();
	IO* self = (IO*)delegate->delegateObject;
	struct IOWrapperDelegate* iOWrapperDelegate = self->__iOWrapperDelegate;
	Queue_Enqueue(self->__out_q, word);
	if (print)
		iOWrapperDelegate->IOWrapperDelegate_Output(iOWrapperDelegate,self->__out_q);
}

static void __Setup_Delegates(IO* self)
{
	_info("Setting up delegates for %s", __FILE__);

	static struct IODelegate ioDelegateVtbl = {
		0,
		&IODelegate_Get_Word_From_Input_Queue,
		&IODelegate_Put_Word_To_Output_Queue,
	};
	ioDelegateVtbl.delegateObject = self;
	self->iODelegateVptr = &ioDelegateVtbl;
}

// Public class methods for IO
// ...

// Public instance methods for IO
// ...
