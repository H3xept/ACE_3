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
* Filename: VMLogger.c
* Class: VMLogger
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-25
* 
* Description: The logger for the virtual machine
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../oop/umbrella.h"
#include "VMLogger.h"
#include "./constants/registers.h"
#include "./constants/arch_const.h"
#include <math.h>

/// The type string of VMLogger
static const char* const 	type_string = "VMLogger";

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

/// Class descriptor structure to instantiate VMLogger
static struct Class_Descriptor _VMLogger_Class_Descriptor = {
	sizeof(VMLogger),
	&vtbl
};
const void * VMLogger_Class_Descriptor = &_VMLogger_Class_Descriptor;

/// Private overrides for 'Object' virtual methods (implementation)

typedef struct {
	uword_t opcode : 4;
	uword_t operand : 12;
} instruction_t;

/**
* @brief: VMLogger constructor.
* @param self: A reference to the current instance of VMLogger
* @param args: Variadic args list as follows:
* - char*: filename for trace file
* - char*: filename for memdump file
* - char*: filename for reg dump file
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	// Downcast to VMLogger
	VMLogger* _self = (VMLogger*)self;
	_self->__trace_file = va_arg(args, char*);
	_self->__memdump_file = va_arg(args, char*);
	_self->__reg_file = va_arg(args, char*);
	// _self->__disasm = alloc_init(Dissassembler_Class_Descriptor);

	return self;
}

/**
* @brief: VMLogger destructor.
* @param self: A reference to the current instance of VMLogger
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	// if (_self->__disasm)
	// 	free(_self->__disasm);
	// _self->__disasm = 0;

	return self;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of VMLogger
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of VMLogger
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	return "<VMLogger>";
}

/**
* @brief: Returns 0 if object is not the same instance as another.
* @param self: A reference to the current instance of VMLogger.
* @param obj: A reference to the another instance of VMLogger.
* @return: unsigned int: 0 if not equal.
*/
static unsigned int _Object_Equals(Object* self, Object* obj)
{
	return Object_Equals(self, obj);
}

/**
* @brief: Gets mnemonic of operation from opcode.
* @param opcode: The opcode to be decoded.
* @return: char*: The mnemonic assosciated with the opcode.
*/
char* __Mnemonic_With_Opcode(uint8_t opcode)
{
	static char* op_mnemonics[] = {"HALT","JUMP","SKC","LOAD","STORE","IN","OUT","MOVE","ADD","MUL","DIV","AND","OR","NOT","SHL","SHR"};
	if (opcode >= sizeof(op_mnemonics)/sizeof(char**))
	{
		_err("Opcode number not valid (%d). Aborting...",opcode);
	}
	return op_mnemonics[opcode];
}

/**
* @brief: Gets mnemonic of register from address.
* @param opcode: The register address to be decoded.
* @return: char*: The mnemonic assosciated with the register.
*/
char* __Mnemonic_With_Register(uint8_t register_n)
{
	static char* reg_mnemonics[] = {"$PC","$IR","$RA","$SP","$FP","$T1","$T2","$T3","$T4","$S1","$S2","$S3","$S4","$S5","$PR","$FR"};
	if (register_n >= sizeof(reg_mnemonics)/sizeof(char**))
	{
		_err("Register number not valid (%d). Aborting...",register_n);
	}
	return reg_mnemonics[register_n];	
}

/**
* @brief: Disassembles instruction that takes no registers.
* @param instruction_t: The instruction to be disassembled.
* @return: char*: The disassembled instruction.
*/
char* __Disassemble_Instruction_With_No_Registers(instruction_t* instruction)
{
	return __Mnemonic_With_Opcode(instruction->opcode);
}

/**
* @brief: Disassembles instruction that takes one register.
* @param instruction_t: The instruction to be disassembled.
* @return: char*: The disassembled instruction.
*/
char* __Disassemble_Instruction_With_One_Register(instruction_t* instruction)
{
	char* op_mnemonic = __Mnemonic_With_Opcode(instruction->opcode);
	uint8_t reg_number = instruction->operand & 0x00f;
	char* ret = calloc(strlen(op_mnemonic)+5, sizeof(char));
	sprintf(ret, "%s %s",op_mnemonic,__Mnemonic_With_Register(reg_number));
	return ret;
}

/**
* @brief: Disassembles instruction that takes two registers.
* @param instruction_t: The instruction to be disassembled.
* @return: char*: The disassembled instruction.
*/
char* __Disassemble_Instruction_With_Two_Registers(instruction_t* instruction)
{
	char* op_mnemonic = __Mnemonic_With_Opcode(instruction->opcode);

	uint8_t reg1_number = (instruction->operand & 0x0f0) >> 4;
	uint8_t reg2_number = instruction->operand & 0x00f;

	char* ret = calloc(strlen(op_mnemonic)+5, sizeof(char));
	sprintf(ret, "%s %s %s",op_mnemonic,__Mnemonic_With_Register(reg1_number), __Mnemonic_With_Register(reg2_number));
	return ret;
}

/**
* @brief: Disassembles a jump instruction.
* @param instruction_t: The instruction to be disassembled.
* @return: char*: The disassembled instruction.
*/
char* __Disassemble_Jump(instruction_t* instruction)
{
	uword_t jump_address = instruction->operand;
	char* ret = calloc(strlen(__Mnemonic_With_Opcode(instruction->opcode))+6, sizeof(char));
	sprintf(ret, "%s 0x%x",__Mnemonic_With_Opcode(instruction->opcode), jump_address);
	return ret;
}

/**
* @brief: Disassembles a load instruction.
* @param instruction_t: The instruction to be disassembled.
* @return: char*: The disassembled instruction.
*/
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

/**
* @brief: Disassembles an out instruction.
* @param instruction_t: The instruction to be disassembled.
* @return: char*: The disassembled instruction.
*/
char* __Disassemble_Out(instruction_t* instruction)
{
	char* op_mnemonic = __Mnemonic_With_Opcode(instruction->opcode);
	uint8_t flag = (instruction->operand & 0x0f0) >> 4;
	uint8_t reg_number = instruction->operand & 0x00f;

	char* ret = calloc(strlen(op_mnemonic)+8, sizeof(char));
	sprintf(ret, "%s %s 0x%x",op_mnemonic,__Mnemonic_With_Register(reg_number), flag > 0);
	return ret;
}

/**
* @brief: Generates an instruction_t from a word.
* @param word: The word to build an instruction from.
* @return: instruction_t*: The instruction generated from the word.
*/
instruction_t* __Instruction_With_Word(uword_t word)
{
	instruction_t* ret = calloc(1, sizeof(instruction_t));
	ret->operand = word&((uword_t)pow(2,WORD_SIZE - OPCODE_LENGTH)-1);
	ret->opcode = word>>(WORD_SIZE - OPCODE_LENGTH);
	return ret;
}

/**
* @brief: Disassembles a given instruction.
* @param word: The instruction to disassemble.
* @return: char*: The disassembled instruction.
*/
char* __Disassemble_Instruction(instruction_t* instruction)
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

// Public instance methods for VMLogger

/**
* @brief: Dumps the current memory of the VM.
* @param self: A reference to the current instance of VMLogger.
* @param memory: A pointer to the current memory.
*/
void VMLogger_Dump_Memory(VMLogger* self, const void* memory)
{
	assert(self->__memdump_file);
	FILE* file = fopen(self->__memdump_file, "wb");
	
	if (!file)
	{
		_err("Could not open memory dump log file.", NULL);
	}

	fwrite(memory, sizeof(uword_t), WORDS_IN_MEM, file);
	fclose(file);
}

/**
* @brief: Dumps the current registers of the VM.
* @param self: A reference to the current instance of VMLogger.
* @param registers: A pointer to the current registers.
*/
void VMLogger_Dump_Registers(VMLogger* self, Registers* registers)
{
	assert(self->__reg_file);

	FILE* file = fopen(self->__reg_file, "w");
	if (!file)
	{
		_err("Could not open register dump log file.", NULL);
	}

	fprintf(file, "PC: 0x%x\n", registers->PC);
	fprintf(file, "IR: 0x%x\n", registers->IR);
	fprintf(file, "RA: 0x%x\n", registers->RA);
	fprintf(file, "SP: 0x%x\n", registers->SP);
	fprintf(file, "FP: 0x%x\n", registers->FP);
	fprintf(file, "T1: 0x%x\n", registers->T1);
	fprintf(file, "T2: 0x%x\n", registers->T2);
	fprintf(file, "T3: 0x%x\n", registers->T3);
	fprintf(file, "T4: 0x%x\n", registers->T4);
	fprintf(file, "S1: 0x%x\n", registers->S1);
	fprintf(file, "S2: 0x%x\n", registers->S2);
	fprintf(file, "S3: 0x%x\n", registers->S3);
	fprintf(file, "S4: 0x%x\n", registers->S4);
	fprintf(file, "S5: 0x%x\n", registers->S5);
	fprintf(file, "PR: 0x%x\n", registers->PR);

	fclose(file);
}

/**
* @brief: Disassembles and writes the given instruction to the trace file.
* @param self: A reference to the current instance of VMLogger.
* @param instruction: Instruction to log.
*/
void VMLogger_Append_New_Binary_Instruction(VMLogger* self, uword_t instruction)
{

	instruction_t _instruction;
	_instruction.operand = instruction&((uword_t)pow(2,WORD_SIZE - OPCODE_LENGTH)-1);
	_instruction.opcode = instruction>>(WORD_SIZE - OPCODE_LENGTH);

	assert(self->__trace_file);
	if (!self->__trace_file_obj)
	{
		self->__trace_file_obj = fopen(self->__trace_file, "w+");
		if (!self->__trace_file_obj) { _err("Could not open trace log file.", NULL); }
	}

	fprintf(self->__trace_file_obj, "%s\n", __Disassemble_Instruction(&_instruction));
}

/**
* @brief: Closes the trace file.
* @param self: A reference to the current instance of VMLogger.
*/
void VMLogger_Close_Trace_File(VMLogger* self)
{
	if (self->__trace_file_obj)
		fclose(self->__trace_file_obj);
}
