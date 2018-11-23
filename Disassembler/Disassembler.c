/**
* Filename: Disassembler.c
* Class: Disassembler
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-22
* 
* Description: A
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../oop/umbrella.h"
#include "../utilities/utilities.h"
#include <math.h>
#include "../VirtualMachine/constants/arch_const.h"
#include "../VirtualMachine/constants/var_word_size.h"
#include "../VirtualMachine/Program.h"
#include <errno.h>
#include "Disassembler.h"

typedef struct {
	uword_t opcode : 4;
	uword_t operand : 12;
} instruction_t;

/// The type string of Disassembler
static const char* const 	type_string = "Disassembler";

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

/// Class descriptor structure to instantiate Disassembler
static struct Class_Descriptor _Disassembler_Class_Descriptor = {
	sizeof(Disassembler),
	&vtbl
};
const void * Disassembler_Class_Descriptor = &_Disassembler_Class_Descriptor;

// Private fields for Disassembler
// ...

// Private class method declarations for Disassembler
// ...

// Private instance method declarations for Disassembler
// ...

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: Disassembler constructor.
* @param self: A reference to the current instance of Disassembler
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	Disassembler* _self = (Disassembler*)self;
	_self->program = va_arg(args, Program*);
	return self;
}

/**
* @brief: Disassembler destructor.
* @param self: A reference to the current instance of Disassembler
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	return self;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of Disassembler
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of Disassembler
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	return "<Disassembler>";
}

static unsigned int _Object_Equals(Object* self, Object* obj)
{
	// Downcast to Disassembler
	Disassembler* _self = (Disassembler*)self;
	Disassembler* _obj = (Disassembler*)obj;

	if (_self->program->size != _obj->program->size) return 0;
	for (int i = 0; i < _self->program->size; i++)
	{
		if (_self->program->source[i] != _obj->program->source[i]) return 0;
	} return 1;
}

// Private class methods for Disassembler
char* __Mnemonic_With_Opcode(uint8_t opcode)
{
	static char* op_mnemonics[] = {"HALT","JUMP","SKC","LOAD","STORE","IN","OUT","MOVE","ADD","MUL","DIV","AND","OR","NOT","SHL","SHR"};
	if (opcode >= sizeof(op_mnemonics)/sizeof(char**))
	{
		_err("Opcode number not valid (%d). Aborting...",opcode);
	}
	return op_mnemonics[opcode];
}


char* __Mnemonic_With_Register(uint8_t register_n)
{
	static char* reg_mnemonics[] = {"$PC","$IR","$RA","$SP","$FP","$T1","$T2","$T3","$T4","$S1","$S2","$S3","$S4","$S5","$PR","$FR"};
	if (register_n >= sizeof(reg_mnemonics)/sizeof(char**))
	{
		_err("Register number not valid (%d). Aborting...",register_n);
	}
	return reg_mnemonics[register_n];	
}

char* __Disassemble_Instruction_With_No_Registers(instruction_t* instruction)
{
	return __Mnemonic_With_Opcode(instruction->opcode);
}

char* __Disassemble_Instruction_With_One_Register(instruction_t* instruction)
{
	char* op_mnemonic = __Mnemonic_With_Opcode(instruction->opcode);
	uint8_t reg_number = instruction->operand & 0x00f;
	char* ret = calloc(strlen(op_mnemonic)+5, sizeof(char));
	sprintf(ret, "%s %s",op_mnemonic,__Mnemonic_With_Register(reg_number));
	return ret;
}

char* __Disassemble_Instruction_With_Two_Registers(instruction_t* instruction)
{
	char* op_mnemonic = __Mnemonic_With_Opcode(instruction->opcode);

	uint8_t reg1_number = (instruction->operand & 0x0f0) >> 4;
	uint8_t reg2_number = instruction->operand & 0x00f;

	char* ret = calloc(strlen(op_mnemonic)+5, sizeof(char));
	sprintf(ret, "%s %s %s",op_mnemonic,__Mnemonic_With_Register(reg1_number), __Mnemonic_With_Register(reg2_number));
	return ret;
}

char* __Disassemble_Jump(instruction_t* instruction)
{
	uword_t jump_address = instruction->operand;
	char* ret = calloc(strlen(__Mnemonic_With_Opcode(instruction->opcode))+6, sizeof(char));
	sprintf(ret, "%s 0x%x",__Mnemonic_With_Opcode(instruction->opcode), jump_address);
	return ret;
}

char* __Disassemble_Load(instruction_t* instruction)
{
	char* op_mnemonic = __Mnemonic_With_Opcode(instruction->opcode);

	uint8_t flag = (instruction->operand & 0x0f0) >> 4;
	
	uint8_t reg1_number = ((instruction->operand & 0xf00) >> 8);
	uint8_t val_2;
	char* ret;

	if (flag <= 0){
		ret = calloc(strlen(op_mnemonic)+8, sizeof(char));
	 	val_2 = instruction->operand & 0x07f;
	 	sprintf(ret, "%s %s %s",op_mnemonic, __Mnemonic_With_Register(reg1_number),
	 		__Mnemonic_With_Register(val_2));
	}
	else {
		ret = calloc(strlen(op_mnemonic)+9, sizeof(char));
		val_2 = (instruction->operand & 0x07f);
	 	sprintf(ret, "%s %s 0x%x",op_mnemonic, __Mnemonic_With_Register(reg1_number),
	 		val_2);
	}

	return ret;
}

char* __Disassemble_Out(instruction_t* instruction)
{
	char* op_mnemonic = __Mnemonic_With_Opcode(instruction->opcode);
	uint8_t flag = (instruction->operand & 0x0f0) >> 4;
	uint8_t reg_number = instruction->operand & 0x00f;

	char* ret = calloc(strlen(op_mnemonic)+8, sizeof(char));
	sprintf(ret, "%s %s 0x%x",op_mnemonic,__Mnemonic_With_Register(reg_number), flag > 0);
	return ret;
}

// Private instance methods for Disassembler
instruction_t* Disassembler_Instruction_With_Word(uword_t word)
{
	instruction_t* ret = calloc(1, sizeof(instruction_t));
	ret->operand = word&((uword_t)pow(2,WORD_SIZE - OPCODE_LENGTH)-1);
	ret->opcode = word>>(WORD_SIZE - OPCODE_LENGTH);
	return ret;
}

char* Disassembler_Disassemble_Instruction(instruction_t* instruction)
{
	switch(instruction->opcode) {
		case 0x0:
			return __Disassemble_Instruction_With_No_Registers(instruction);
		case 0x2:
		case 0x5:
		case 0xd:
			return __Disassemble_Instruction_With_One_Register(instruction);
		case 0x4:
		case 0x7:
		case 0x8:
		case 0x9:
		case 0xa:
		case 0xb:
		case 0xc:
		case 0xe:
		case 0xf:
			return __Disassemble_Instruction_With_Two_Registers(instruction);
		case 0x1:
			return __Disassemble_Jump(instruction);
		case 0x3:
			return __Disassemble_Load(instruction);
		case 0x6:
			return __Disassemble_Out(instruction);
		default:
			_err("Instruction '%s' not found in arch.", __Mnemonic_With_Opcode(instruction->opcode));
	}
}

// Public class methods for Disassembler
Disassembler* Disassembler_With_Program(Program* program)
{
	Disassembler* disassembler = alloc_init(Disassembler_Class_Descriptor,program);
	return disassembler;
}

// Public instance methods for Disassembler
char** Disassembler_Statically_Disassemble(Disassembler* self)
{
	// Program* program = self->program;
	// uword_t* binary_words = program->source;
	// char** ret = calloc(program->size, sizeof(uword_t));

	// for (int line_n = 0; line_n < program->size; ++line_n)
	// {
	// 	uword_t current_word = *(binary_words+line_n);

	// }
	Program* program = self->program;
	instruction_t* instruction;
	for(int i = 0; i < program->size; i++)
	{
		instruction = Disassembler_Instruction_With_Word(program->source[i]);
		printf("%s\n",Disassembler_Disassemble_Instruction(instruction));
		free(instruction);
		instruction = 0;
	}
	return NULL;
}
