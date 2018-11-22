/**
* Filename: VirtualMachine.c
* Class: VirtualMachine
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-20
* 
* Description: The encapsulating virtual machine object
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../oop/umbrella.h"
#include "../utilities/utilities.h"
#include "CPU.h"
#include "./protocols/IOWrapperDelegate.h"
#include "VirtualMachine.h"
#include <math.h>
#include "./protocols/MemoryDelegate.h"
#include "./constants/arch_const.h"
#include "Program.h"

/// The type string of VirtualMachine
static const char* const 	type_string = "VirtualMachine";

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

/// Class descriptor structure to instantiate VirtualMachine
static struct Class_Descriptor _VirtualMachine_Class_Descriptor = {
	sizeof(VirtualMachine),
	&vtbl
};
const void * VirtualMachine_Class_Descriptor = &_VirtualMachine_Class_Descriptor;

// Private fields for VirtualMachine
// ...

// Private class method declarations for VirtualMachine
// ...

// Private instance method declarations for VirtualMachine
static void __Setup_Delegates(VirtualMachine* self);
static void __Load_Bootloader(VirtualMachine* self);
/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: VirtualMachine constructor.
* @param self: A reference to the current instance of VirtualMachine
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	VirtualMachine* _self = (VirtualMachine*)self;
	_self->cpu = alloc_init(CPU_Class_Descriptor, _self);
	__Setup_Delegates(_self);
	_self->cpu_mode = CPU_Mode_Idle;
	return self;
}

/**
* @brief: VirtualMachine destructor.
* @param self: A reference to the current instance of VirtualMachine
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	VirtualMachine* _self = (VirtualMachine*)self;
	if (_self->cpu)
		free(_self->cpu); _self->cpu = 0;
	return self;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of VirtualMachine
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of VirtualMachine
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	return "<VirtualMachine>";
}

static unsigned int _Object_Equals(Object* self, Object* obj)
{
	return Object_Equals(self,obj);
}

// Private class methods for VirtualMachine
// ...

// Private instance methods for VirtualMachine

#warning Implement
uword_t* IOWrapperDelegate_Input(struct IOWrapperDelegate * delegate)
{
	_delegCall();
	return NULL;
}

void IOWrapperDelegate_Output(struct IOWrapperDelegate * delegate, Queue* output_queue)
{
	_delegCall();
	_info("Output!", NULL);
}


static void __Setup_Delegates(VirtualMachine* self)
{
	_info("Setting up delegates for %s", __FILE__);

	static struct IOWrapperDelegate iOWrapperDelegate = {
		0,
		&IOWrapperDelegate_Input,
		&IOWrapperDelegate_Output,
	};
	iOWrapperDelegate.delegateObject = self;
	self->iOWrapperDelegateVptr = &iOWrapperDelegate;
}

static void __Load_Bootloader(VirtualMachine* self)
{
	self->cpu_mode = CPU_Mode_Booting;
	struct MemoryDelegate* memoryDelegate = self->cpu->memoryDelegateVptr;

	uword_t bootloader[] = {0x0000,0x3981,0x3a81,0x3b82,0x3c86,0x705f,0xf05b,0xb05a,0x2005,0x1001,0x5005,0x4095,0x809a,0x705f,0xf05b,0xb05a,0x2005,0x1ffc,0x1ff3,0x809a,0x7049,0x7039,0x1001};

	size_t bootloader_instr_n = sizeof(bootloader)/sizeof(uword_t);
	for (int i = 0; i < bootloader_instr_n; i++)
		memoryDelegate->MemoryDelegate_Set_Word_At_Address(memoryDelegate,WORDS_IN_MEM-bootloader_instr_n+i,bootloader[i]);
	memoryDelegate->MemoryDelegate_Set_Word_At_Address(memoryDelegate,0,0x1fea);

	_info("Bootloader loading complete.",NULL);
}

static void __Load_Program(VirtualMachine* self, Program* program)
{
	CPU_Load_Words_In_Input_Queue(self->cpu, program->source, program->size);
}

// Public class methods for VirtualMachine
// ...

// Public instance methods for VirtualMachine
void Virtual_Machine_Run(VirtualMachine* self, Program* program)
{	
	if (!program)
	{
		_err("Must specify a program to run.", NULL);
	}

	__Load_Program(self, program);
	__Load_Bootloader(self);
	CPU_Fetch_Execute_Cycle(self->cpu);
}
