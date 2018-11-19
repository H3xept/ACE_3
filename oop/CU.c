/**
* Filename: CU.c
* Class: CU
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-16
* 
* Description: The control unit of the architecture
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "Object.h"
#include "OOP.h"
#include "../utilities/utilities.h"
#include "CU.h"
#include "./protocols/FlagDelegate.h"
#include "./protocols/IODelegate.h"
#include "./protocols/MemoryDelegate.h"
#include "ALU.h"
#include "./constants/registers.h"
#include "./constants/var_word_size.h"

/// The type string of CU
static const char* const 	type_string = "CU";

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

/// Class descriptor structure to instantiate CU
static struct Class_Descriptor _CU_Class_Descriptor = {
	sizeof(CU),
	&vtbl
};
const void * CU_Class_Descriptor = &_CU_Class_Descriptor;

// Private fields for CU

// Private class method declarations for CU
// ...

// Private instance method declarations for CU
static void __HALT(CU* self);
static void __JUMP(CU* self, uword_t operand);
static void __SKC(CU* self, uword_t operand);
static void __LOAD(CU* self, uword_t operand);
static void __STORE(CU* self, uword_t operand);
static void __IN(CU* self, uword_t operand);
static void __OUT(CU* self, uword_t operand);
static void __MOVE(CU* self, uword_t operand);
static void __ADD(CU* self, uword_t operand);
static void __MUL(CU* self, uword_t operand);
static void __DIV(CU* self, uword_t operand);
static void __AND(CU* self, uword_t operand);
static void __OR(CU* self, uword_t operand);
static void __NOT(CU* self, uword_t operand);
static void __SHL(CU* self, uword_t operand);
static void __SHR(CU* self, uword_t operand);
static void __set_register_with_address(CU* self, uint8_t address, uword_t value);
static uword_t __get_register_value_with_address(CU* self, uint8_t address);

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: CU constructor.
* @param self: A reference to the current instance of CU
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	// Downcast to CU
	CU* _self = (CU*)self;

	_self->__registers = va_arg(args, Registers*);
	_self->__alu = va_arg(args, ALU*);
	_self->__ioDelegate = va_arg(args, struct IODelegate*);
	_self->__flagDelegate = va_arg(args, struct FlagDelegate*);
	_self->__memoryDelegate = va_arg(args, struct MemoryDelegate*);

	return self;
}

/**
* @brief: CU destructor.
* @param self: A reference to the current instance of CU
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	return self;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of CU
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of CU
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	return "<ControlUnit>";
}

static unsigned int _Object_Equals(Object* self, Object* obj)
{
	return Object_Equals(self,obj);
}

// Private class methods for CU
// ...

// Private instance methods for CU
static void __HALT(CU* self){
	_info("Executing HALT instruction", NULL);
	struct FlagDelegate* flagDelegate = self->__flagDelegate;
	flagDelegate->FlagDelegate_Set_Flag(flagDelegate, k_Status_Flag_Exit_Code, k_Exit_Code_Halt);
	flagDelegate->FlagDelegate_Set_Flag(flagDelegate, k_Status_Flag_Halt, 1);
}

static void __JUMP(CU* self, uword_t operand){
	_info("Executing JUMP instruction", NULL);
	Registers* registers = self->__registers;
	operand--;
	registers->PC = operand;
}

static void __SKC(CU* self, uword_t operand){
	_info("Executing SKC instruction", NULL);
	Registers * registers = self->__registers;
	if(__get_register_value_with_address(self, (uint8_t)operand) > 0){
		registers->PC++;
	}
}

static void __LOAD(CU* self, uword_t operand){
	_info("Executing LOAD instruction", NULL);
	Registers* registers = self->__registers;
	struct MemoryDelegate* memoryDelegate = self->__memoryDelegate;
	uword_t operand2 = memoryDelegate->MemoryDelegate_Word_At_Address(memoryDelegate, ++(registers->PC));
	if((operand >> REGISTER_ADDR_LENGTH) > 0){
		__set_register_with_address(self,(uint8_t) (operand & ((uword_t)(pow(2, REGISTER_ADDR_LENGTH) - 1))), operand2);
	}
	else{
		uword_t wordToBeWritten = memoryDelegate->MemoryDelegate_Word_At_Address(memoryDelegate, operand2 & ((uword_t)pow(2, ADDR_LENGTH) - 1));
		__set_register_with_address(self,(uint8_t) (operand & ((uword_t)(pow(2, REGISTER_ADDR_LENGTH) - 1))), wordToBeWritten);
	}
}

static void __STORE(CU* self, uword_t operand){
	_info("Executing STORE instruction", NULL);
	Registers* registers = self->__registers;
	struct MemoryDelegate* memoryDelegate = self->__memoryDelegate;
	uword_t address = memoryDelegate->MemoryDelegate_Word_At_Address(memoryDelegate, ++(registers->PC));
	uword_t wordToBeWritten = __get_register_value_with_address(self,(uint8_t) (operand & ((uword_t)(pow(2, REGISTER_ADDR_LENGTH) - 1))));
	memoryDelegate->MemoryDelegate_Set_Word_At_Address(memoryDelegate, address & ((uword_t)pow(2, ADDR_LENGTH) - 1), wordToBeWritten);
}

static void __IN(CU* self, uword_t operand){
	_info("Executing IN instruction", NULL);
	struct IODelegate* ioDelegate = self->__ioDelegate;
	uint8_t address = (uint8_t) (operand & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1));
	__set_register_with_address(self, address, ioDelegate->IODelegate_Get_Word_From_Input_Queue(ioDelegate));
}

static void __OUT(CU* self, uword_t operand){
	_info("Executing OUT instruction", NULL);
	struct IODelegate* ioDelegate = self->__ioDelegate;
	uint8_t address = (uint8_t) (operand & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1));
	ioDelegate->IODelegate_Put_Word_To_Output_Queue(ioDelegate, __get_register_value_with_address(self, address), (operand >> REGISTER_ADDR_LENGTH) > 0);
}


static void __MOVE(CU* self, uword_t operand){
	_info("Executing MOVE instruction", NULL);
	uint8_t address1 = (uint8_t) ((operand >> REGISTER_ADDR_LENGTH) & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1));
	uint8_t address2 = (uint8_t) (operand & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1));
	__set_register_with_address(self, address1, __get_register_value_with_address(self, address2));
}

static void __ADD(CU* self, uword_t operand){
	_info("Executing ADD instruction", NULL);
	ALU* alu = self->__alu;
	uint8_t address = (uint8_t) ((operand >> REGISTER_ADDR_LENGTH) & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1));
	uword_t value1Unsigned = __get_register_value_with_address(self, address);
	uword_t value2Unsigned = __get_register_value_with_address(self, (uint8_t) (operand & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1)));

	word_t result = ALU_Add(alu, unsigned_to_signed(value1Unsigned), unsigned_to_signed(value2Unsigned));
	__set_register_with_address(self, address, signed_to_unsigned(result));
}

static void __MUL(CU* self, uword_t operand){
	_info("Executing MUL instruction", NULL);
	ALU* alu = self->__alu;
	uint8_t address = (uint8_t) ((operand >> REGISTER_ADDR_LENGTH) & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1));
	uword_t value1Unsigned = __get_register_value_with_address(self, address);
	uword_t value2Unsigned = __get_register_value_with_address(self, (uint8_t) (operand & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1)));

	word_t result = ALU_Multiply(alu, unsigned_to_signed(value1Unsigned), unsigned_to_signed(value2Unsigned));
	__set_register_with_address(self, address, signed_to_unsigned(result));
}

static void __DIV(CU* self, uword_t operand){
	_info("Executing DIV instruction", NULL);
	ALU* alu = self->__alu;
	uint8_t address = (uint8_t) ((operand >> REGISTER_ADDR_LENGTH) & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1));
	uword_t value1Unsigned = __get_register_value_with_address(self, address);
	uword_t value2Unsigned = __get_register_value_with_address(self, (uint8_t) (operand & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1)));

	word_t result = ALU_Divide(alu, unsigned_to_signed(value1Unsigned), unsigned_to_signed(value2Unsigned));
	__set_register_with_address(self, address, signed_to_unsigned(result));
}

static void __AND(CU* self, uword_t operand){
	_info("Executing AND instruction", NULL);
	ALU* alu = self->__alu;
	uint8_t address = (uint8_t) ((operand >> REGISTER_ADDR_LENGTH) & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1));
	uword_t value1Unsigned = __get_register_value_with_address(self, address);
	uword_t value2Unsigned = __get_register_value_with_address(self, (uint8_t) (operand & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1)));

	word_t result = ALU_Bitwise_And(alu, unsigned_to_signed(value1Unsigned), unsigned_to_signed(value2Unsigned));
	__set_register_with_address(self, address, signed_to_unsigned(result));
}

static void __OR(CU* self, uword_t operand){
	_info("Executing OR instruction", NULL);
	ALU* alu = self->__alu;
	uint8_t address = (uint8_t) ((operand >> REGISTER_ADDR_LENGTH) & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1));
	uword_t value1Unsigned = __get_register_value_with_address(self, address);
	uword_t value2Unsigned = __get_register_value_with_address(self, (uint8_t) (operand & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1)));

	word_t result = ALU_Bitwise_Or(alu, unsigned_to_signed(value1Unsigned), unsigned_to_signed(value2Unsigned));
	__set_register_with_address(self, address, signed_to_unsigned(result));
}

static void __NOT(CU* self, uword_t operand){
	_info("Executing NOT instruction", NULL);
	ALU* alu = self->__alu;
	uint8_t address = (uint8_t) ((operand >> REGISTER_ADDR_LENGTH) & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1));
	uword_t valueUnsigned = __get_register_value_with_address(self, address);

	word_t result = ALU_Bitwise_Not(alu, unsigned_to_signed(valueUnsigned));
	__set_register_with_address(self, address, signed_to_unsigned(result));
}

static void __SHL(CU* self, uword_t operand){
	_info("Executing SHL instruction", NULL);
	ALU* alu = self->__alu;
	uint8_t address = (uint8_t) ((operand >> REGISTER_ADDR_LENGTH) & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1));
	uword_t value1Unsigned = __get_register_value_with_address(self, address);
	uword_t value2Unsigned = __get_register_value_with_address(self, (uint8_t) (operand & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1)));

	word_t result = ALU_Shift_Left_Logical(alu, unsigned_to_signed(value1Unsigned), value2Unsigned);
	__set_register_with_address(self, address, signed_to_unsigned(result));
}

static void __SHR(CU* self, uword_t operand){
	_info("Executing SHR instruction", NULL);
	ALU* alu = self->__alu;
	uint8_t address = (uint8_t) ((operand >> REGISTER_ADDR_LENGTH) & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1));
	uword_t value1Unsigned = __get_register_value_with_address(self, address);
	uword_t value2Unsigned = __get_register_value_with_address(self, (uint8_t) (operand & ((uword_t)pow(2, REGISTER_ADDR_LENGTH) - 1)));

	word_t result = ALU_Shift_Right_Logical(alu, unsigned_to_signed(value1Unsigned), value2Unsigned);
	__set_register_with_address(self, address, signed_to_unsigned(result));
}

static void __set_register_with_address(CU* self, uint8_t address, uword_t value){
	_info("Setting register (Address: %d)", address);
	Registers* registers = self->__registers;
	uword_t mask = ((uword_t)pow(2,ADDR_LENGTH) - 1);
	switch(address){
		case 0:
			registers->PC = value & mask;
			break;
		case 1:
			registers->IR = value;
			break;
		case 2:
			registers->RA = value & mask;
			break;
		case 3:
			registers->SP = value & mask;
			break;
		case 4:
			registers->FP = value & mask;
			break;
		case 5:
			registers->T1 = value;
			break;
		case 6:
			registers->T2 = value;
			break;
		case 7:
			registers->T3 = value;
			break;
		case 8:
			registers->T4 = value;
			break;
		case 9:
			registers->S1 = value;
			break;
		case 10:
			registers->S2 = value;
			break;
		case 11:
			registers->S3 = value;
			break;
		case 12:
			registers->S4 = value;
			break;
		case 13:
			registers->S5 = value;
			break;
		case 14:
			registers->PR = value;
			break;
		case 15:
			registers->PC = value;
			break;
		default:
			_warn("Illegal register access (Address: %d)", address);
			struct FlagDelegate* flagDelegate = self->__flagDelegate;
			flagDelegate->FlagDelegate_Set_Flag(flagDelegate, k_Status_Flag_Exit_Code, 0);
			flagDelegate->FlagDelegate_Set_Flag(flagDelegate, k_Status_Flag_Halt, 2);

	}
}

static uword_t __get_register_value_with_address(CU* self, uint8_t address) {
	Registers* registers = self->__registers;
	switch(address){
		case 0:
			return registers->PC;
		case 1:
			return registers->IR;
		case 2:
			return registers->RA;
		case 3:
			return registers->SP;
		case 4:
			return registers->FP;
		case 5:
			return registers->T1;
		case 6:
			return registers->T2;
		case 7:
			return registers->T3;
		case 8:
			return registers->T4;
		case 9:
			return registers->S1;
		case 10:
			return registers->S2;
		case 11:
			return registers->S3;
		case 12:
			return registers->S4;
		case 13:
			return registers->S5;
		case 14:
			return registers->PR;
		case 15:
			return registers->PC;
		default:
			_warn("Illegal register access (Address: %d)", address);
			struct FlagDelegate* flagDelegate = self->__flagDelegate;
			flagDelegate->FlagDelegate_Set_Flag(flagDelegate, k_Status_Flag_Exit_Code, k_Exit_Code_Illegal_Register_Access);
			flagDelegate->FlagDelegate_Set_Flag(flagDelegate, k_Status_Flag_Halt, 1);
			return 0;
	}
}

// Public class methods for CU
// ...

// Public instance methods for CU
void CU_Execute_Instruction(CU* self, instruction_t instruction){
	_info("Executing instruction (Opcode: %d, Operand: %d)", instruction.opcode, instruction.operand);
	Registers* registers = self->__registers;
	registers->IR = (instruction.opcode << (WORD_SIZE - OPCODE_LENGTH)) + instruction.operand;
	switch(instruction.opcode){
		case 0:
			__HALT(self);
			break;
		case 1:
			__JUMP(self, instruction.operand);
			break;
		case 2:
			__SKC(self, instruction.operand);
			break;
		case 3:
			__LOAD(self, instruction.operand);
			break;
		case 4:
			__STORE(self, instruction.operand);
			break;
		case 5:
			__IN(self, instruction.operand);
			break;
		case 6:
			__OUT(self, instruction.operand);
			break;
		case 7:
			__MOVE(self, instruction.operand);
			break;
		case 8:
			__ADD(self, instruction.operand);
			break;
		case 9:
			__MUL(self, instruction.operand);
			break;
		case 10:
			__DIV(self, instruction.operand);
			break;
		case 11:
			__AND(self, instruction.operand);
			break;
		case 12:
			__OR(self, instruction.operand);
			break;
		case 13:
			__NOT(self, instruction.operand);
			break;
		case 14:
			__SHL(self, instruction.operand);
			break;
		case 15:
			__SHR(self, instruction.operand);
	}
	registers->PC++;
}
