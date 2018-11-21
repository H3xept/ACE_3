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
static uword_t bin_String_To_Word(char* input,int len){
	int dec = 0;
	for(int i = 0; i < len; i++)
		dec += (input[i] - '0') * pow(2,(len-1-i));	
	return dec;
}

static uword_t hex_String_To_Word(char*input, int len){
	int dec = 0;
	for(int i = 0; i < len; i++){
		if (((input[i] - '0') >= 0) && ((input[i] - '0') <= 9))
			dec += (input[i] - '0') * pow(16,(len-1-i));
		if (((input[i] - 'a' + 10) >= 10) && ((input[i] - 'a' + 10) <= 15))
			dec += (input[i] - 'a' + 10) * pow(16,(len-1-i));
		if (((input[i] - 'A' + 10) >= 10) && ((input[i] - 'A' + 10) <= 15))
			dec += (input[i] - 'A' + 10) * pow(16,(len-1-i));
	}
	return dec;
}

static unsigned long long int_String_To_Word(char* input,int len){
	int dec = 0;
	for(int i = 0; i < len; i++)
		dec+= (input[i] - '0') * pow(10,(len-1-i));	
	return dec;
}

static uword_t is_Binary(char* bin,int len){
	for(int c = 0;c < strlen(bin); c++){
		if((bin[c]- '0') < 0 || (bin[c]- '0') > 1)
			return 0;
	}
	return 1;
}

static uword_t is_Hexidecimal(char* hex,int len){
	for(int c = 0;c < strlen(hex); c++){
		if(!((((hex[c]- 'a') >= 0) && ((hex[c]- 'a') <= 5))
			|| (((hex[c]- 'A') >= 0) && ((hex[c]- 'A') <= 5))
			|| (((hex[c] - '0') >= 0) && ((hex[c] - '0') <= 9))))
			return 0;
	}
	return 1;
}

static uword_t check_Only_Digits(char* string,int len){
	for (int i = 0; i < len; i++){
		if(((string[i] - '0') < 0) || ((string[i] - '0') > 9))
			return 0;
	}
	return 1;
}

#warning initial implementation
uword_t* IOWrapperDelegate_Input(struct IOWrapperDelegate * delegate)
{
	_delegCall();
	char* word;
	uword_t words[MAX_INPUT_LEN/WORD_SIZE];
	char input_string[MAX_INPUT_LEN];
	
	fgets(input_string,MAX_INPUT_LEN-1,stdin);
	
	for(int i = 0; i < MAX_INPUT_LEN; i++) {
		if(input_string[i] == '\n')
			input_string[i] = '\0';
	}
		
	int i = 1;	
	word = strtok(input_string, " ");
	
	while( word != NULL ) {
		if (strlen(word) <= WORD_SIZE+2 && word[0] == '0' && word[1] == 'b'){
			char* bin = word+2;
			if(!is_Binary(bin,strlen(bin)))
				_err("invalid characters in binary input\n",NULL);
			words[i] = bin_String_To_Word(bin,strlen(bin));
			i++;
		}
		else if((strlen(word) <= (WORD_SIZE/4)+2) && word[0] == '0' && (word[1] == 'x' || word[1] == 'X')){
			char* hex = word+2;
			if(!is_Hexidecimal(hex,strlen(hex)))
				_err("invalid characters in hexadecimal input\n",NULL);
			words[i] = hex_String_To_Word(hex,strlen(hex));
			i++;
		}
		else if((strlen(word) <= 10) && check_Only_Digits(word,strlen(word))){
			long long num = int_String_To_Word(word,strlen(word));
			if (num > UWORD_MAX)
				_err("integer value too large\n",NULL);
			words[i] = num;
			i++;
		}
		else{
			_err("invalid input, expected hex(0x prefix), binary(0b prefix) or unsigned integer value\n",NULL);
		}
		word = strtok(NULL, " ");
	}
	
	words[0] = i-1;
	return words;
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

	uword_t bootloader[] = {0x0000, 0x3019, 0x0001, 0x301a, 0x0001, 0x301b, 0x0002, 0x301c,
						0x0006, 0x705f, 0xf05b, 0xb05a, 0x2005, 0x1001, 0x5005, 0x4095,
						0x809a, 0x705f, 0xf05b, 0xb05a, 0x2005, 0x1001, 0x1ff7};


	size_t bootloader_instr_n = sizeof(bootloader)/sizeof(uword_t);
	for (int i = 0; i < bootloader_instr_n; i++)
		memoryDelegate->MemoryDelegate_Set_Word_At_Address(memoryDelegate,WORDS_IN_MEM-bootloader_instr_n+i,bootloader[i]);
	memoryDelegate->MemoryDelegate_Set_Word_At_Address(memoryDelegate,0,0x1fea);

	_info("Bootloader loading complete.",NULL);
}

static void __Load_Program(VirtualMachine* self, uword_t* program, size_t word_n)
{
	CPU_Load_Words_In_Input_Queue(self->cpu, program, word_n);
	// for (int i = 0; i < word_n; i++)
	// {
	// 	_info("--> %d",*(self->cpu->__iOController->__in_q->Q+i));
	// }
}

// Public class methods for VirtualMachine
// ...

// Public instance methods for VirtualMachine
void Virtual_Machine_Run(VirtualMachine* self, uword_t* program, size_t word_n)
{
	__Load_Program(self, program, word_n);
	__Load_Bootloader(self);
	CPU_Fetch_Execute_Cycle(self->cpu);
}
