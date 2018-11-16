/**
* Filename: CPU.c
* Class: CPU
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-16
* 
* Description: __DESCRIPTION__
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "Object.h"
#include "OOP.h"
#include "CPU.h"
#include "ALU.h"
#include "CU.h"
#include "../utilities/utilities.h"
#include "./constants/registers.h"
#include "./constants/flag_register.h"
#include "./protocols/FlagDelegate.h"
#include "./protocols/MemoryDelegate.h" // Acting as an adaptor
#include "./protocols/IODelegate.h" // Acting as an adaptor

/// The type string of CPU
static const char* const 	type_string = "CPU";

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

/// Class descriptor structure to instantiate CPU
static struct Class_Descriptor _CPU_Class_Descriptor = {
	sizeof(CPU),
	&vtbl
};
const void * CPU_Class_Descriptor = &_CPU_Class_Descriptor;

// Private fields for CPU
// ...

// Private class method declarations for CPU
// ...

// Private instance method declarations for CPU

static void __Fetch_Execute_Cycle(CPU* self);
static inline void __Setup_Delegates(CPU* self);
static inline void __CPU_Init_Registers(CPU* self);
static CU* __Construct_Control_Unit(CPU* self, ...);
/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: CPU constructor.
* @param self: A reference to the current instance of CPU
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	// Downcast to CPU
	CPU* _self = (CPU*)self;

	self->__registers = malloc(sizeof(Registers));
	self->__flagRegister = malloc(sizeof(Flag_Register));
	__CPU_Init_Registers(_self);
	__CPU_Init_Flag_Register(_self);

	self->__alu = alloc_init(ALU_Class_Descriptor);
	self->__controlUnit = __Construct_Control_Unit(self,self->__registers,self->__alu,_self,_self,_self);

	self->__memoryController = alloc_init(MemoryController_Class_Descriptor);
	self->__iOController = alloc_init(IO_Class_Descriptor);

	return self;
}

/**
* @brief: CPU destructor.
* @param self: A reference to the current instance of CPU
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	// Downcast to CPU
	CPU* _self = (CPU*)self;
	if (_self->__alu){
		Object_Dtor((Object*)_self->__alu);
		free(_self->__alu);
		_self->__alu = 0;
	}

	if (_self->__controlUnit){
		Object_Dtor((Object*)_self->__controlUnit);
		free(_self->__controlUnit);
		_self->__controlUnit = 0;
	}

	if (_self->__registers)
		free(_self->__registers); _self->__registers = 0;
	if (_self->__flagRegister)
		free(_self->__flagRegister); _self->__flagRegister = 0;

	if (_self->__memoryController){
		Object_Dtor((Object*)_self->__memoryController);
		free(_self->__memoryController);
		_self->__memoryController = 0;
	}

	if (_self->__iOController){
		Object_Dtor((Object*)_self->__iOController);
		free(_self->__iOController);
		_self->__iOController = 0;
	}

	return self;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of CPU
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of CPU
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	return "<CPU>";
}

static unsigned int _Object_Equals(Object* self, Object* obj)
{
	return Object_Equals(self,obj);
}

// Private class methods for CPU

// Private instance methods for CPU

// MemoryDelegate ADAPTOR
static int16_t MemoryDelegate_Word_At_Address(struct MemoryDelegate* self, int16_t addr)
{	
	_info("%s Received delegate call -> %s",__FILE__, __func__);
	MemoryDelegate* delegate = ((CPU*)self)->__memoryController->memoryDelegateVptr; // Explicit downcast
	return delegate->MemoryDelegate_Word_At_Address(delegate, addr);
}

static void MemoryDelegate_Set_Word_At_Address(struct MemoryDelegate* self, int16_t addr, int16_t word)
{	
	_info("%s Received delegate call -> %s",__FILE__, __func__);
	MemoryDelegate* delegate = ((CPU*)self)->__memoryController->memoryDelegateVptr; // Explicit downcast
	delegate->MemoryDelegate_Set_Word_At_Address(delegate, addr, word);
}

static void MemoryDelegate_Clear_Memory(struct MemoryDelegate* self)
{	
	_info("%s Received delegate call -> %s",__FILE__, __func__);
	MemoryDelegate* delegate = ((CPU*)self)->__memoryController->memoryDelegateVptr; // Explicit downcast
	delegate->MemoryDelegate_Clear_Memory(delegate);
}

static void MemoryDelegate_Load_Memory_From_Ptr(struct MemoryDelegate* self, void* ptr, size_t size)
{	
	_info("%s Received delegate call -> %s",__FILE__, __func__);
	MemoryDelegate* delegate = ((CPU*)self)->__memoryController->memoryDelegateVptr; // Explicit downcast
	delegate->MemoryDelegate_Load_Memory_From_Ptr(delegate, ptr, size);
}
// ---

// IODelegate ADAPTOR
static word_t IODelegate_Get_Word_From_Input_Queue(struct IODelegate * delegate)
{
	_info("%s Received delegate call -> %s",__FILE__, __func__);
	IODelegate* delegate = ((CPU*)self)->__iOController->iODelegateVptr; // Explicit downcast
	return delegate->IODelegate_Get_Word_From_Input_Queue(delegate);
}

static void IODelegate_Put_Word_To_Output_Queue(struct IODelegate * delegate, word_t word, uint8_t print)
{
	_info("%s Received delegate call -> %s",__FILE__, __func__);
	IODelegate* delegate = ((CPU*)self)->__iOController->memoryDelegateVptr; // Explicit downcast
	delegate->IODelegate_Put_Word_To_Output_Queue(delegate, word, print);
}
// ---

// FlagDelegate 
static void FlagDelegate_Set_Flag(struct FlagDelegate * delegate, k_Status_Flag flag, uint8_t value)
{
	_info("%s Received delegate call -> %s",__FILE__, __func__);
	CPU* self = (CPU*)delegate; // Explicit Cast
	switch(flag)
	{
		case k_Status_Flag_Halt:
			self->__flagRegister->halt = value & 0b1;
			break;
		case k_Status_Flag_Overflow:
			self->__flagRegister->overflow = value & 0b1;
			break;
		case k_Status_Flag_Input:
			self->__flagRegister->input = value & 0b1;
			break;
		case k_Status_Flag_Exit_Codes:
			self->__flagRegister->exit_code = value & 0b11;
			break;
		default:
			_err("The requested flag does not exist: %d",flag);
	}
}

static uword_t FlagDelegate_Get_Flags_As_Word(struct FlagDelegate * delegate)
{
	_info("%s Received delegate call -> %s",__FILE__, __func__);
	CPU* self = (CPU*)delegate; // Explicit Cast
	Flag_Register* reg = self->__flagRegister;
	return 0 | reg->halt | reg->overflow | reg->input | reg->exit_code;
}

static uint8_t FlagDelegate_Read_Flag(struct FlagDelegate * delegate, k_Status_Flag flag)
{
	_info("%s Received delegate call -> %s",__FILE__, __func__);
	CPU* self = (CPU*)delegate; // Explicit Cast
	Flag_Register* reg = self->__flagRegister;
	switch(flag)
	{
		case k_Status_Flag_Halt:
			return (uint8_t)reg->halt;
		case k_Status_Flag_Overflow:
			return (uint8_t)reg->overflow;
		case k_Status_Flag_Input:
			return (uint8_t)reg->input;
		case k_Status_Flag_Exit_Codes:
			return (uint8_t)reg->exit_code;
		default:
			_err("The requested flag does not exist: %d",flag);
	}
}
// ---

static void __Setup_Delegates(CPU* self)
{
	static struct MemoryDelegate memoryDelegateVtbl = {
		&MemoryDelegate_Word_At_Address,
		&MemoryDelegate_Set_Word_At_Address,
		&MemoryDelegate_Clear_Memory,
		&MemoryDelegate_Load_Memory_From_Ptr
	};
	self->memoryDelegateVptr = &memoryDelegateVtbl;

	static struct FlagDelegate flagDelegateVtbl = {
		&IODelegate_Get_Word_From_Input_Queue,
		&IODelegate_Put_Word_To_Output_Queue
	}
	self->flagDelegateVptr = &flagDelegateVtbl;

	static struct FlagDelegate iODelegateVtbl = {
		&FlagDelegate_Set_Flag,
		&FlagDelegate_Get_Flags_As_Word,
		&FlagDelegate_Read_Flag
	}
	self->iODelegateVptr = &iODelegateVtbl;
}

static inline void __CPU_Init_Registers(CPU* self)
{
	self->PC = 0;
	self->IR = 0;
	self->RA = 0;
	self->SP = 0;
	self->FP = 0;
	self->T1 = 0;
	self->T2 = 0;
	self->T3 = 0;
	self->T4 = 0;
	self->S1 = 0;
	self->S2 = 0;
	self->S3 = 0;
	self->S4 = 0;
	self->S5 = 0;
	self->PR = 0;
}

static inline void __CPU_Init_Flag_Register(CPU* self) 
{
	self->halt = 0;
	self->overflow = 0;
	self->input = 0;
	self->exit_code = 0;
}

static CU* __Construct_Control_Unit(CPU* self, ...)
{	
	va_list args;
	va_start(args,self);
	CPU* ret = alloc_init(CU_Class_Descriptor, args);
	va_end(args);
	return ret;
}

#warning Temporary
static void __Fetch_Execute_Cycle(CPU* self)
{	
	MemoryDelegate* delegate = (MemoryDelegate*)self->__memoryController;
	while(!(self->__flagRegister->halt))
	{
		uword_t pc_word = delegate->MemoryDelegate_Word_At_Address(delegate,self->__registers->PC);
		CU_Execute_Function(self->__controlUnit);
	}
}

// Public class methods for CPU
// ...

// Public instance methods for CPU
// ...
