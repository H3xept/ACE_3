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
#include "VMLogger.h"

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
	_self->__logger = va_arg(args, VMLogger*);

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
/**
* @brief: retrieves word at address.
* @param IODelegate: A reference to the implementer of this method
* @param addr: memory address to fetch from
* @return uint16/32 word at the specified address.
*/
static uword_t MemoryDelegate_Word_At_Address(struct MemoryDelegate* delegate, uword_t addr)
{	
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	struct MemoryDelegate* adaptedDelegate = self->__memoryController->memoryDelegateVptr; // Explicit downcast
	return adaptedDelegate->MemoryDelegate_Word_At_Address(adaptedDelegate, addr);
}

/**
* @brief: sets word at address.
* @param IODelegate: A reference to the implementer of this method
* @param addr: memory address to set word
* @param word: value to set
* @return uint16/32 word at the specified address.
*/
static void MemoryDelegate_Set_Word_At_Address(struct MemoryDelegate* delegate, uword_t addr, uword_t word)
{	
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	struct MemoryDelegate* adaptedDelegate = self->__memoryController->memoryDelegateVptr; // Explicit downcast
	adaptedDelegate->MemoryDelegate_Set_Word_At_Address(adaptedDelegate, addr, word);
}

/**
* @brief: sets all memory values to 0.
* @param IODelegate: A reference to the implementer of this method
*/
static void MemoryDelegate_Clear_Memory(struct MemoryDelegate* delegate)
{	
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	struct MemoryDelegate* adaptedDelegate = self->__memoryController->memoryDelegateVptr; // Explicit downcast
	adaptedDelegate->MemoryDelegate_Clear_Memory(adaptedDelegate);
}


/**
* @brief: loads memory from location ptr.
* @param self: A reference to the current instance of Memorycontroller
* @param ptr: location to get words from
* @param size: number of words to be loaded to memory
*/
static void MemoryDelegate_Load_Memory_From_Ptr(struct MemoryDelegate* delegate, void* ptr, size_t size)
{	
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	struct MemoryDelegate* adaptedDelegate = self->__memoryController->memoryDelegateVptr; // Explicit downcast
	adaptedDelegate->MemoryDelegate_Load_Memory_From_Ptr(adaptedDelegate, ptr, size);
}

/**
* @brief: Dumps memory.
* @param IODelegate: A reference to the implementer of this method
*/
static const void* MemoryDelegate_Dump_Readonly_Memory(struct MemoryDelegate* delegate)
{	
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	struct MemoryDelegate* adaptedDelegate = self->__memoryController->memoryDelegateVptr; // Explicit downcast
	return adaptedDelegate->MemoryDelegate_Dump_Readonly_Memory(adaptedDelegate);
}

// ---

// IODelegate ADAPTOR
/**
* @brief: Returns a word from the input queue.
* @param IODelegate: A reference to the implementer of this method
* @return: uword_t - first word from the input queue.
*/
static uword_t IODelegate_Get_Word_From_Input_Queue(struct IODelegate * delegate)
{
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	struct IODelegate* adaptedDelegate = self->__iOController->iODelegateVptr; // Explicit downcast
	return adaptedDelegate->IODelegate_Get_Word_From_Input_Queue(adaptedDelegate);
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
	CPU* self = (CPU*)(delegate->delegateObject);
	struct IODelegate* adaptedDelegate = self->__iOController->iODelegateVptr; // Explicit downcast
	adaptedDelegate->IODelegate_Put_Word_To_Output_Queue(adaptedDelegate, word, print);
}
// ---

// FlagDelegate 
/**
* @brief: sets a flag to specified value.
* @param FlagDelegate: A reference to the implementer of this method
* @param flag: flag to set
* @param value: value to be set
*/
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

/**
* @brief: returns word representing flag values.
* @param FlagDelegate: A reference to the implementer of this method
* @return the word representation of the flags
*/
static uword_t FlagDelegate_Get_Flags_As_Word(struct FlagDelegate * delegate)
{
	_delegCall();
	CPU* self = (CPU*)(delegate->delegateObject);
	FlagRegister* reg = self->__flagRegister;

	return (reg->halt << 4) | (reg->overflow << 3) | (reg->input << 2) | reg->exit_code;
}

/**
* @brief: returns value of specified flag.
* @param FlagDelegate: A reference to the implementer of this method
* @param flag: name of flag to read
* @return uint8_t: value of flag 
*/
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
		&MemoryDelegate_Load_Memory_From_Ptr,
		&MemoryDelegate_Dump_Readonly_Memory
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

///initialises registers to 0
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

///initialises flags to 0
static inline void __CPU_Init_Flag_Register(CPU* self) 
{
	self->__flagRegister->halt = 0;
	self->__flagRegister->overflow = 0;
	self->__flagRegister->input = 0;
	self->__flagRegister->exit_code = 0;
}

// Public class methods for CPU
// ...

// Public instance methods for CPU
/**
* @brief: simulates FEC using the components of the cpu.
* @param self: reference to the current instance of CPU
*/
void CPU_Fetch_Execute_Cycle(CPU* self)
{		
	_info("Starting FEC", NULL);

	struct MemoryDelegate* memoryDelegate = (struct MemoryDelegate*)(self->memoryDelegateVptr);
	struct FlagDelegate* flagDelegate = (struct FlagDelegate*)(self->flagDelegateVptr);
	
	instruction_t instruction = {0};

	self->vm->cpu_mode = CPU_Mode_Running;
	while(!FlagDelegate_Read_Flag(flagDelegate,k_Status_Flag_Halt))
	{	
		uword_t pc_word = MemoryDelegate_Word_At_Address(memoryDelegate,self->__registers->PC);
		instruction.operand = pc_word&((uword_t)pow(2,WORD_SIZE - OPCODE_LENGTH)-1);
		instruction.opcode = pc_word>>(WORD_SIZE - OPCODE_LENGTH);

		if (self->__logger)
			VMLogger_Append_New_Binary_Instruction(self->__logger, pc_word);

		CU_Execute_Instruction(self->__controlUnit, instruction);
	}

	if (self->__logger)
	{
		VMLogger_Close_Trace_File(self->__logger);
		VMLogger_Dump_Memory(self->__logger, 
			MemoryDelegate_Dump_Readonly_Memory(memoryDelegate));
		VMLogger_Dump_Registers(self->__logger,self->__registers);
	}

	self->vm->cpu_mode = CPU_Mode_Idle;

	__CPU_Init_Registers(self);
	__CPU_Init_Flag_Register(self);
}

// WARNING - Viable only during CPU idle stage
/**
* @brief: put words into the input queue.
* @param words: array of words to load
* @param word_n: number of words to load
*/
void CPU_Load_Words_In_Input_Queue(CPU* self, uword_t* words, size_t word_n)
{
	struct FlagDelegate* flagDelegate = self->flagDelegateVptr;

	if (self->vm->cpu_mode == CPU_Mode_Idle)
	{
		for (int i = 0; i < word_n; i++)
			Queue_Enqueue(self->__iOController->__in_q,*(words+i));
		if (word_n)
			FlagDelegate_Set_Flag(flagDelegate,k_Status_Flag_Input,1);
		return;
	} 
	_err("Direct access to the input queue is denied if not in idle stage.", NULL);
}
