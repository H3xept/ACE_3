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
#include <errno.h>

#define MAX_INPUT_LEN 200

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

// Private instance method declarations for VirtualMachine
static void __Setup_Delegates(VirtualMachine* self);
static void __Load_Bootloader(VirtualMachine* self);

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: VirtualMachine constructor.
* @param self: A reference to the current instance of VirtualMachine
* @param args: No variadic args
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	VirtualMachine* _self = (VirtualMachine*)self;
	__Setup_Delegates(_self);
	_self->cpu = alloc_init(CPU_Class_Descriptor, _self);
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

/**
* @brief: Returns 0 if object is not the same instance as another.
* @param self: A reference to the current instance of VirtualMachine.
* @param obj: A reference to the another instance of VirtualMachine.
* @return: unsigned int: 0 if not equal.
*/
static unsigned int _Object_Equals(Object* self, Object* obj)
{
	return Object_Equals(self,obj);
}

// Private instance methods for VirtualMachine

/**
* @brief: takes user input in hex, bin or decimal.
* @param IOWrapperDelegate: A reference to the implementer of this method
* @return array of unsigned words where first value is the number of words returned
*/
uword_t* IOWrapperDelegate_Input(struct IOWrapperDelegate * delegate)
{
	_delegCall();
	char* word;
	int i = 1;	
	long int signed_word;
	uword_t* words = malloc(sizeof(word_t) * (MAX_INPUT_LEN/WORD_SIZE));
	char input_string[MAX_INPUT_LEN];

	uword_t max_accepted = (uword_t)pow(2, WORD_SIZE-1)-1;
	word_t min_accepted = (word_t)(pow(2, WORD_SIZE-1)*-1);

	printf("VirtualMachine~$ ");
	
	if (!strcmp("\n",fgets(input_string,MAX_INPUT_LEN-1,stdin)))
	{
		_warn("Input required.",NULL);
		return IOWrapperDelegate_Input(delegate);
	}

	input_string[strlen(input_string)-1] = input_string[strlen(input_string)-1] == '\n' ? '\0' : 
	input_string[strlen(input_string)-1];
		
	word = strtok(input_string, " ");
	
	while( word != NULL ) 
	{
		errno = 0;
		if (*word == '0' && *(word+1)=='b')
		{
			word += 2;
			signed_word = strtol(word, NULL, 2);
		}
		else
		{
			signed_word = strtol(word, NULL, 0);
		}

		if (errno){
			_warn("Please insert a number in decimal, binary(0b prefix) or hex(0x prefix).",NULL);
			return IOWrapperDelegate_Input(delegate);
		}

		if (signed_word > max_accepted || signed_word < min_accepted)
		{
			_warn("Immediate out of bounds %d (MAX %d | MIN %d).", signed_word, max_accepted, min_accepted);
			return IOWrapperDelegate_Input(delegate);
		}
		words[i++] = signed_word;
		word = strtok(NULL, " ");
	}
	
	words[0] = i-1;
	return words;
}

/**
* @brief: prints contents of output queue (and empties output queue).
* @param IOWrapperDelegate: A reference to the implementer of this method
* @param ouput_queue: the ouput queue
*/
void IOWrapperDelegate_Output(struct IOWrapperDelegate * delegate, Queue* output_queue)
{
	_delegCall();
	word_t word;
	while (!Queue_Is_Empty(output_queue))
	{
		word = Queue_Dequeue(output_queue);
		printf("%d ",word);
	}puts("");
}

/**
* @brief: Sets up delegates for VirtualMachine
* @param self: reference to the current instance of VirtualMachine
*/
static void __Setup_Delegates(VirtualMachine* self)
{
	_info("Setting up delegates for %s", __FILE__);

	static struct IOWrapperDelegate iOWrapperDelegateVtbl = {
		0,
		&IOWrapperDelegate_Input,
		&IOWrapperDelegate_Output,
	};
	iOWrapperDelegateVtbl.delegateObject = self;
	self->iOWrapperDelegateVptr = &iOWrapperDelegateVtbl;
}

/**
* @brief: prints contents of output queue (and empties output queue).
* @param IOWrapperDelegate: A reference to the implementer of this method
* @param ouput_queue: the ouput queue
*/
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

/**
* @brief: loads a program into the input queue.
* @param VitrualMachine: A reference to an instance of VirtualMachine
* @param program: pointer to the program instructions
*/
static void __Load_Program(VirtualMachine* self, Program* program)
{
	CPU_Load_Words_In_Input_Queue(self->cpu, program->source, program->size);
}

// Public instance methods for VirtualMachine

/**
* @brief: Runs the virtual machine with a given program
* @param self: A reference to an instance of VirtualMachine
* @param program: pointer to the program instructions
*/
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
