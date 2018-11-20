/**
* Filename: CPU.c
* Class: CPU
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-16
* 
* Description: The CPU of the machine.
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "../oop/umbrella.h"
#include "CPU.h"
#include "ALU.h"
#include "CU.h"
#include <math.h>
#include "./constants/var_word_size.h"
#include "./constants/registers.h"
#include "./constants/flag_register.h"
#include "./protocols/FlagDelegate.h"
#include "./protocols/MemoryDelegate.h" // Acting as an adaptor
#include "./protocols/IODelegate.h" // Acting as an adaptor
#include "VirtualMachine.h"

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
static inline void __CPU_Init_Registers(CPU* self);
static void __Setup_Delegates(CPU* self);
static inline void __CPU_Init_Flag_Register(CPU* self);

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
	__Setup_Delegates(_self);

	_self->vm = va_arg(args, VirtualMachine*);
	assert(_self->vm);
	#warning Delete

	_self->__registers = malloc(sizeof(Registers));
	_self->__flagRegister = malloc(sizeof(FlagRegister));
	__CPU_Init_Registers(_self);
	__CPU_Init_Flag_Register(_self);

	_self->__alu = alloc_init(ALU_Class_Descriptor);
	_self->__controlUnit = alloc_init(CU_Class_Descriptor, 
													_self->__registers, 
													_self->__alu,
													_self->iODelegateVptr, 
													_self->flagDelegateVptr, 
													_self->memoryDelegateVptr);
	_self->__memoryController = alloc_init(MemoryController_Class_Descriptor);
	_self->__iOController = alloc_init(IO_Class_Descriptor, _self->flagDelegateVptr, _self->vm->iOWrapperDelegateVptr);
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
static uword_t MemoryDelegate_Word_At_Address(struct MemoryDelegate* delegate, uword_t addr)
{	
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	struct MemoryDelegate* adaptedDelegate = self->__memoryController->memoryDelegateVptr; // Explicit downcast
	return adaptedDelegate->MemoryDelegate_Word_At_Address(adaptedDelegate, addr);
}

static void MemoryDelegate_Set_Word_At_Address(struct MemoryDelegate* delegate, uword_t addr, uword_t word)
{	
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	struct MemoryDelegate* adaptedDelegate = self->__memoryController->memoryDelegateVptr; // Explicit downcast
	adaptedDelegate->MemoryDelegate_Set_Word_At_Address(adaptedDelegate, addr, word);
}

static void MemoryDelegate_Clear_Memory(struct MemoryDelegate* delegate)
{	
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	struct MemoryDelegate* adaptedDelegate = self->__memoryController->memoryDelegateVptr; // Explicit downcast
	adaptedDelegate->MemoryDelegate_Clear_Memory(adaptedDelegate);
}

static void MemoryDelegate_Load_Memory_From_Ptr(struct MemoryDelegate* delegate, void* ptr, size_t size)
{	
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	struct MemoryDelegate* adaptedDelegate = self->__memoryController->memoryDelegateVptr; // Explicit downcast
	adaptedDelegate->MemoryDelegate_Load_Memory_From_Ptr(adaptedDelegate, ptr, size);
}
// ---

// IODelegate ADAPTOR
static uword_t IODelegate_Get_Word_From_Input_Queue(struct IODelegate * delegate)
{
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	struct IODelegate* adaptedDelegate = self->__iOController->iODelegateVptr; // Explicit downcast
	return adaptedDelegate->IODelegate_Get_Word_From_Input_Queue(adaptedDelegate);
}

static void IODelegate_Put_Word_To_Output_Queue(struct IODelegate * delegate, uword_t word, uint8_t print)
{
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	struct IODelegate* adaptedDelegate = self->__iOController->iODelegateVptr; // Explicit downcast
	adaptedDelegate->IODelegate_Put_Word_To_Output_Queue(adaptedDelegate, word, print);
}
// ---

// FlagDelegate 
static void FlagDelegate_Set_Flag(struct FlagDelegate * delegate, k_Status_Flag flag, uint8_t value)
{
	// _info("%s Received delegate call -> %s",__FILE__, __func__);
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	switch(flag)
	{
		case k_Status_Flag_Halt:
			self->__flagRegister->halt = value & 1;
			break;
		case k_Status_Flag_Overflow:
			self->__flagRegister->overflow = value & 1;
			break;
		case k_Status_Flag_Input:
			self->__flagRegister->input = value & 1;
			break;
		case k_Status_Flag_Exit_Code:
			self->__flagRegister->exit_code = value & 3;
			break;
		default:
			_err("The requested flag does not exist: %d",flag);
	}
}

static uword_t FlagDelegate_Get_Flags_As_Word(struct FlagDelegate * delegate)
{
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	FlagRegister* reg = self->__flagRegister;

	return (reg->halt << 4) | (reg->overflow << 3) | (reg->input << 2) | reg->exit_code;
}

static uint8_t FlagDelegate_Read_Flag(struct FlagDelegate * delegate, k_Status_Flag flag)
{
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	FlagRegister* reg = self->__flagRegister;
	switch(flag)
	{
		case k_Status_Flag_Halt:
			return (uint8_t)reg->halt;
		case k_Status_Flag_Overflow:
			return (uint8_t)reg->overflow;
		case k_Status_Flag_Input:
			return (uint8_t)reg->input;
		case k_Status_Flag_Exit_Code:
			return (uint8_t)reg->exit_code;
		default:
			_err("The requested flag does not exist: %d",flag);
	}
}
// ---

static void __Setup_Delegates(CPU* self)
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

	static struct IODelegate iODelegateVtbl = {
		0,
		&IODelegate_Get_Word_From_Input_Queue,
		&IODelegate_Put_Word_To_Output_Queue
	};
	iODelegateVtbl.delegateObject = self;
	self->iODelegateVptr = &iODelegateVtbl;

	static struct FlagDelegate flagDelegateVtbl = {
		0,
		&FlagDelegate_Set_Flag,
		&FlagDelegate_Get_Flags_As_Word,
		&FlagDelegate_Read_Flag
	};
	flagDelegateVtbl.delegateObject = self;
	self->flagDelegateVptr = &flagDelegateVtbl;
}

static inline void __CPU_Init_Registers(CPU* self)
{
	self->__registers->PC = 0;
	self->__registers->IR = 0;
	self->__registers->RA = 0;
	self->__registers->SP = 0;
	self->__registers->FP = 0;
	self->__registers->T1 = 0;
	self->__registers->T2 = 0;
	self->__registers->T3 = 0;
	self->__registers->T4 = 0;
	self->__registers->S1 = 0;
	self->__registers->S2 = 0;
	self->__registers->S3 = 0;
	self->__registers->S4 = 0;
	self->__registers->S5 = 0;
	self->__registers->PR = 0;
}

static inline void __CPU_Init_Flag_Register(CPU* self) 
{
	self->__flagRegister->halt = 0;
	self->__flagRegister->overflow = 0;
	self->__flagRegister->input = 0;
	self->__flagRegister->exit_code = 0;
}

#warning Temporary
void CPU_Fetch_Execute_Cycle(CPU* self)
{		
	_info("Starting FEC", NULL);

	struct MemoryDelegate* memoryDelegate = (struct MemoryDelegate*)(self->memoryDelegateVptr);
	struct FlagDelegate* flagDelegate = (struct FlagDelegate*)(self->flagDelegateVptr);
	
	instruction_t instruction = {0};
	
	while(!FlagDelegate_Read_Flag(flagDelegate,k_Status_Flag_Halt))
	{	
		uword_t pc_word = MemoryDelegate_Word_At_Address(memoryDelegate,self->__registers->PC);
		instruction.operand = pc_word&((uword_t)pow(2,WORD_SIZE - OPCODE_LENGTH)-1);
		instruction.opcode = pc_word>>(WORD_SIZE - OPCODE_LENGTH);
		CU_Execute_Instruction(self->__controlUnit, instruction);
	}
}

// Public class methods for CPU
// ...

// Public instance methods for CPU
// ...