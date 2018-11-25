/**
* Filename: Disassembler.c
* Class: Disassembler
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-22
* 
* Description: A disassembler for TBTTBSQSA (The Bigger Than Tiny But Still Quite Small Architecture)
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

	_self->is_done_disassembling = 0;
	_self->__labels_n = 0;
	_self->__current_label = '\0';
	_self->__skc_stack = alloc_init(Stack_Class_Descriptor, 200);
	_self->__stored_ra_stack = alloc_init(Stack_Class_Descriptor, 200);
	_self->__ra_stack = alloc_init(Stack_Class_Descriptor, 200);
	_self->__label_list = calloc(MAX_LABELS, sizeof(Label_Pair));

	return self;
}

/**
* @brief: Disassembler destructor.
* @param self: A reference to the current instance of Disassembler
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	Disassembler* _self = (Disassembler*)self;

	if (_self->__skc_stack){
		Object_Dtor((Object*)_self->__skc_stack);
		free(_self->__skc_stack);
		_self->__skc_stack = 0;
	}

	if (_self->__ra_stack){
		Object_Dtor((Object*)_self->__ra_stack);
		free(_self->__ra_stack);
		_self->__ra_stack = 0;
	}

	if (_self->__stored_ra_stack){
		Object_Dtor((Object*)_self->__stored_ra_stack);
		free(_self->__stored_ra_stack);
		_self->__stored_ra_stack = 0;
	}

	if (_self->__label_list){
		free(_self->__label_list);
		_self->__label_list = 0;
	}

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
static char* __Mnemonic_With_Opcode(uint8_t opcode)
{
	static char* op_mnemonics[] = {"HALT","JUMP","SKC","LOAD","STORE",
	"IN","OUT","MOVE","ADD","MUL","DIV","AND","OR","NOT","SHL","SHR"};

	if (opcode >= sizeof(op_mnemonics)/sizeof(char**))
	{
		_err("Opcode number not valid (%d). Aborting...",opcode);
	}
	return op_mnemonics[opcode];
}


static char* __Mnemonic_With_Register(uint8_t register_n)
{
	static char* reg_mnemonics[] = {"$PC","$IR","$RA","$SP","$FP",
	"$T1","$T2","$T3","$T4","$S1","$S2","$S3","$S4","$S5","$PR","$FR"};

	if (register_n >= sizeof(reg_mnemonics)/sizeof(char**))
	{
		_err("Register number not valid (%d). Aborting...",register_n);
	}
	return reg_mnemonics[register_n];	
}

// Private instance methods for Disassembler

static instruction_t __Instruction_With_Word(Disassembler* self, uword_t word)
{
	instruction_t ret = {0};
	ret.operand = word&((uword_t)pow(2,WORD_SIZE - OPCODE_LENGTH)-1);
	ret.opcode = word>>(WORD_SIZE - OPCODE_LENGTH);
	return ret;
}

static char __Next_Label(Disassembler* self, uword_t label_address)
{	
	if (!self->__current_label)
		self->__current_label = 'A';

	Label_Pair lp = {self->__current_label, label_address};
	*(self->__label_list + self->__labels_n++) = lp;
	return self->__current_label++;
}

static uword_t __Kill_Thread(Disassembler* self)
{
	Stack* ra_stack = NULL;
	SKC_Pair *pair = NULL;
	uword_t ret = 0;
	Stack* skc_stack = self->__skc_stack;

	if (Stack_Is_Empty(skc_stack)) {
		self->is_done_disassembling = 1;
		return 0;
	}

	ra_stack = self->__ra_stack;
	pair = Stack_Pop_Typed(skc_stack, SKC_Pair*);

	for (int pop_n = ra_stack->length - pair->ra_stack_length; pop_n > 0 ; --pop_n){

		int* popped_ra_stack = Stack_Pop_Typed(ra_stack, int*);
		int* examined_stored_ra = Stack_Examine_Typed(self->__stored_ra_stack, int*);

		if (examined_stored_ra) {
			if(*examined_stored_ra == *popped_ra_stack) 
				Stack_Pop_Typed(self->__stored_ra_stack, int*);
		}

		if (popped_ra_stack){
			free(popped_ra_stack);
		}
		popped_ra_stack = 0;
	}

	ret = pair->branch_address;

	if (pair)
		free(pair);
	pair = 0;

	return ret;
}


static char* __Disassemble_Instruction_With_No_Registers(Disassembler* self, instruction_t* instruction)
{
	return __Mnemonic_With_Opcode(instruction->opcode);
}

static char* __Disassemble_Instruction_With_One_Register(Disassembler* self, instruction_t* instruction)
{
	char* op_mnemonic = __Mnemonic_With_Opcode(instruction->opcode);
	uint8_t reg_number = instruction->operand & 0x00f;
	char* ret = calloc(strlen(op_mnemonic)+5, sizeof(char));
	sprintf(ret, "%s %s",op_mnemonic,__Mnemonic_With_Register(reg_number));
	return ret;
}

static char* __Disassemble_Instruction_With_Two_Registers(Disassembler* self, instruction_t* instruction)
{
	char* op_mnemonic = __Mnemonic_With_Opcode(instruction->opcode);

	uint8_t reg1_number = (instruction->operand & 0x0f0) >> 4;
	uint8_t reg2_number = instruction->operand & 0x00f;

	char* ret = calloc(strlen(op_mnemonic)+5, sizeof(char));
	sprintf(ret, "%s %s %s",op_mnemonic,__Mnemonic_With_Register(reg1_number), __Mnemonic_With_Register(reg2_number));
	return ret;
}

static char* __Disassemble_Jump(Disassembler* self, instruction_t* instruction, uword_t current_address, uword_t* new_address)
{
	uword_t jump_address = instruction->operand;

	char* ret = calloc(strlen(__Mnemonic_With_Opcode(instruction->opcode))+6, sizeof(char));
	if (jump_address == 0xfff){

		int* new_addr_from_ra = Stack_Pop_Typed(self->__ra_stack,int*);
		sprintf(ret, "%s 0xfff",__Mnemonic_With_Opcode(instruction->opcode));

		*new_address = *new_addr_from_ra;
		if (new_addr_from_ra)
			free(new_addr_from_ra);
		new_addr_from_ra = 0;

	} else {

		int* current_address_cpy = calloc(1, sizeof(int));
		*current_address_cpy = current_address+1;
		Stack_Push(self->__ra_stack, current_address_cpy);

		for (int i = 0; i < self->__labels_n; i++)
		{
			Label_Pair pair = self->__label_list[i];
			if (pair.label_address == jump_address){
				sprintf(ret, "%s %c",__Mnemonic_With_Opcode(instruction->opcode), pair.label);		
				return ret;
			}
		}

		sprintf(ret, "%s %c",__Mnemonic_With_Opcode(instruction->opcode), __Next_Label(self, jump_address));
		*new_address = jump_address;

	}
	return ret;
}

static char* __Disassemble_Load(Disassembler* self, instruction_t* instruction)
{
	char* op_mnemonic = __Mnemonic_With_Opcode(instruction->opcode);

	uint8_t flag = (instruction->operand & 0x0f0) >> 4;
	
	uint8_t reg1_number = ((instruction->operand & 0xf00) >> 8);
	uint8_t val_2;
	char* ret;
	char* mnemonic_reg_1 = __Mnemonic_With_Register(reg1_number);

	if (flag <= 0)
	{
		int* examine_ra_stack = Stack_Examine_Typed(self->__stored_ra_stack, int*);
		int* examine_stored_ra_stack = Stack_Examine_Typed(self->__ra_stack, int*);

		if (examine_ra_stack && examine_stored_ra_stack)
		{
			if (!strcmp(mnemonic_reg_1, "$RA") && *examine_ra_stack != *examine_stored_ra_stack) 
			{
				int* popped_stored_ra = Stack_Pop_Typed(self->__stored_ra_stack, int*);
				while(*Stack_Examine_Typed(self->__ra_stack, int*) != *popped_stored_ra 
					&& *popped_stored_ra) 
				{
					int * popped_ra = Stack_Pop_Typed(self->__ra_stack, int*);
					if (popped_ra)
						free(popped_ra);
					popped_ra = 0;
				}
			}
		}

		ret = calloc(strlen(op_mnemonic)+8, sizeof(char));
	 	val_2 = instruction->operand & 0x07f;
	 	sprintf(ret, "%s %s %s",op_mnemonic, mnemonic_reg_1,
	 		__Mnemonic_With_Register(val_2));
	}
	else 
	{
		ret = calloc(strlen(op_mnemonic)+9, sizeof(char));
		val_2 = (instruction->operand & 0x07f);
	 	sprintf(ret, "%s %s %d",op_mnemonic, __Mnemonic_With_Register(reg1_number),
	 		(val_2 > 64) ? val_2 - 128 : val_2);
	}

	return ret;
}

static char* __Disassemble_Out(Disassembler* self, instruction_t* instruction)
{
	char* op_mnemonic = __Mnemonic_With_Opcode(instruction->opcode);
	uint8_t flag = (instruction->operand & 0x0f0) >> 4;
	uint8_t reg_number = instruction->operand & 0x00f;

	char* ret = calloc(strlen(op_mnemonic)+8, sizeof(char));
	sprintf(ret, "%s %s 0x%x",op_mnemonic,__Mnemonic_With_Register(reg_number), flag > 0);
	return ret;
}

static char** __Finalize_Disassembly(Disassembler* self, char** without_labels)
{
	char** ret = calloc(self->program->size+self->__labels_n, sizeof(char*));
	for (int i = 0, labels_inserted = 0; i < self->program->size; ++i)
	{
		for (int j = 0; j < self->__labels_n; ++j)
		{
			Label_Pair pair = self->__label_list[j];
			if (pair.label_address == i+1) 
			{
				char* formatted_label = calloc(3,sizeof(char));
				sprintf(formatted_label, "%c:", pair.label);
				ret[i+labels_inserted++] = formatted_label;
			}
		}
		ret[labels_inserted+i] = without_labels[i+1];
	}
	return ret;
}

static char** __Insert_Data(Disassembler* self, char** without_data)
{
	size_t size = self->program->size+self->__labels_n;
	for (int i = 1; i < size+1; ++i)
	{
		if (*(without_data+i) == NULL)
		{
			uword_t current_line = *(self->program->source+i-1);

			*(without_data+i) = calloc(6,sizeof(char));
			sprintf(*(without_data+i),"%d",
				current_line > (int)pow(2,WORD_SIZE-1) ? 
				current_line - (int)pow(2,WORD_SIZE) : current_line);
		}
	}
	return without_data; // But with data!
}

static void __Extra_Conditions_For_Halt(Disassembler* self, instruction_t* instruction, uword_t* new_address)
{
	*new_address = __Kill_Thread(self);
}

// Public class methods for Disassembler
Disassembler* Disassembler_With_Program(Program* program)
{
	Disassembler* disassembler = alloc_init(Disassembler_Class_Descriptor,program);
	return disassembler;
}

// Public instance methods for Disassembler

char* Disassembler_Disassemble_Instruction(Disassembler* self, instruction_t* instruction, uword_t current_address, uword_t* new_address)
{	
	char* ret;
	SKC_Pair* pair;
	int* stored_ra_int; 

	switch(instruction->opcode) {
		case 0x0: // HLT
			ret =__Disassemble_Instruction_With_No_Registers(self, instruction);
			__Extra_Conditions_For_Halt(self, instruction, new_address);
			break;
		case 0x2: // SKC
			pair = calloc(1, sizeof(SKC_Pair));
			pair->branch_address = current_address+2;
			pair->ra_stack_length = self->__ra_stack->length;
			Stack_Push(self->__skc_stack, pair);
		case 0x5:
		case 0xd:
			ret =__Disassemble_Instruction_With_One_Register(self, instruction);
			break;
		case 0x4: // Store
			if (!strcmp(__Mnemonic_With_Register((instruction->operand & 0x00f)),"$RA"))
			{
				stored_ra_int = calloc(1, sizeof(int));
				stored_ra_int = Stack_Examine_Typed(self->__ra_stack, int*);
				Stack_Push(self->__stored_ra_stack, stored_ra_int);
			}
		case 0x7:
		case 0x8:
		case 0x9:
		case 0xa:
		case 0xb:
		case 0xc:
		case 0xe:
		case 0xf:
			ret =__Disassemble_Instruction_With_Two_Registers(self, instruction);
			break;
		case 0x1:
			ret =__Disassemble_Jump(self, instruction, current_address, new_address);
			break;
		case 0x3:
			ret =__Disassemble_Load(self, instruction);
			break;
		case 0x6:
			ret =__Disassemble_Out(self, instruction);
			break;
		default:
			_err("Instruction '%s' not found in arch.", __Mnemonic_With_Opcode(instruction->opcode));
	}
	return ret;
}

char** Disassembler_Statically_Disassemble(Disassembler* self, int* size)
{
	char** disassembled = calloc(WORDS_IN_MEM + MAX_LABELS, sizeof(char*));
	char* disasm_instruction;
	char** ret;
	uword_t new_address = 0;

	Program* program = self->program;
	instruction_t instruction;

	uword_t current_address = 1;
	while(!self->is_done_disassembling)
	{
		if (*(disassembled+current_address)) {
			current_address = __Kill_Thread(self);
			continue;
		}

		new_address = current_address+1;
		instruction = __Instruction_With_Word(self, program->source[current_address-1]);
		disasm_instruction = Disassembler_Disassemble_Instruction(self, &instruction, current_address, &new_address);
		disassembled[current_address] = disasm_instruction;
		current_address = new_address;
	}


	ret = __Finalize_Disassembly(self, __Insert_Data(self,disassembled));

	if (disassembled)
		free(disassembled);
	disassembled = 0;

	*size = self->program->size+self->__labels_n;

	return ret;
}

void Disassembler_Statically_Disassemble_And_Dump(Disassembler* self, int* size, char* filename)
{
	FILE* file = fopen(filename, "w");
	if (!file) { _err("Can't open file %s.",filename); }

	char** dump = Disassembler_Statically_Disassemble(self, size);
	for (int i = 0; i < *size; ++i)
		fprintf(file, "%s\n", *(dump+i));

	fclose(file);
}
