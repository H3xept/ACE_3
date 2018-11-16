/**
* Filename: ALU.c
* Class: ALU
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-12
* 
* Description: the place where arithmetic and logical operations take place
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Object.h"
#include "OOP.h"
#include "../utilities/utilities.h"
#include "ALU.h"

#include "./constants/arch_const.h"

/// The type string of ALU
static const char* const 	type_string = "ALU";

/// Private overrides for 'Object' virtual methods (signature)
static Object* 			 	_Object_Ctor(Object * self, va_list args);
static Object* 			 	_Object_Dtor(Object * self);
static const char* const 	_Object_Type_Descriptor(Object * _self);
static const char* const 	_Object_Descriptor(Object * self);

/// Function binding for virtual methods table
static struct ObjectVTbl 	vtbl = {
								&_Object_Ctor,
								&_Object_Dtor,
								&_Object_Type_Descriptor,
								&_Object_Descriptor
							};

/// Class descriptor structure to instantiate ALU
static struct Class_Descriptor _ALU_Class_Descriptor = {
	sizeof(ALU),
	&vtbl
};
const void * ALU_Class_Descriptor = &_ALU_Class_Descriptor;

// private fields
// ...

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: ALU constructor.
* @param self: A reference to the current instance of ALU
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	ALU* _self = (ALU*)self;
	_self->flags.halt = 0;
	_self->flags.overflow = 0;
	_self->flags.input = 0;
	_self->flags.exit_code = 0;
	return self;
}

/**
* @brief: ALU destructor.
* @param self: A reference to the current instance of ALU
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	return self;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of ALU
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of ALU
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	return "<ALU>";
}

// Private class methods for ALU
// ...

// Private instance methods for ALU
// ...

// Public class methods for ALU
// ...

// Public instance methods for ALU

//returns sum of two 16bit ints
int16_t ALU_Add(ALU* self, int16_t num_1, int16_t num_2)
{
	int16_t ret = 0;
	if ((num_1 > 0 && num_2 > INT16_MAX - num_1) || 
		(num_1 < 0 && num_2 < INT16_MIN - num_1)) {
		_warn("Overflow detected!", NULL);
		uint16_t tnum_1 = 1+UINT16_MAX+num_1;
		uint16_t tnum_2 = 1+UINT16_MAX+num_2;
		ret = 0 | (tnum_2+tnum_1);
		self->flags.overflow = 1;
	}
	else{
		self->flags.overflow = 0;
		ret = num_1+num_2;
	}
	return ret;
}

//returns product of two 16bit ints
//DETECTING OVERFLOW? ALSO SIZE OF PRODUCT TOO BIG?(I think this works...)
int16_t ALU_Multiply(ALU* self, int16_t num_1, int16_t num_2)
{
	unsigned int overflow = 0;
	int16_t ret = 0;

	if (num_1 > 0)
	{
		if (num_2 > 0 && num_1 > INT16_MAX/num_2){
			overflow = 1;
		} else if(num_1 < INT16_MIN/num_2) {
			overflow = 1;
		}
	} else {
		if (num_2 > 0 && num_2 < INT16_MIN/num_1) {
			overflow = 1;
		} else if (num_1 != 0 && num_2 < INT16_MAX/num_1) {
			overflow = 1;
		}
	} self->flags.overflow = overflow;

	if (overflow) {
		_warn("Overflow detected!", NULL);
		uint16_t tnum_1 = 1+UINT16_MAX+num_1;
		uint16_t tnum_2 = 1+UINT16_MAX+num_2;
		ret = 0 | tnum_2*tnum_1;
	} else {
		ret = num_1*num_2;
	}
	return ret;
}

//returns result of first argument divided by second argument
int16_t ALU_Divide(ALU* self, int16_t num_1, int16_t num_2)
{
	if (num_2 == 0) 
		_err("Division by zero! Aborting...", NULL);
	if (num_1 == INT16_MIN && num_2 == -1) {
		_warn("Overflow detected!", NULL);
		self->flags.overflow = 1;
		return num_1;
	} self->flags.overflow = 0;
	return num_1/num_2;
}

//bitwise AND of two 16bit ints 
int16_t ALU_Bitwise_And(int16_t num_1, int16_t num_2)
{
	return num_1 & num_2;
}

//bitwise OR of two 16bit ints 
int16_t ALU_Bitwise_Or(ALU* self, int16_t num_1, int16_t num_2)
{
	return num_1 | num_2;
}

int16_t ALU_Bitwise_Not(ALU* self, int16_t num)
{
	return ~num;
}

int16_t ALU_Shift_Left_Logical(ALU* self, int16_t num_1, uint16_t num_2)
{
	return num_1 << num_2;
}

int16_t ALU_Shift_Right_Logical(ALU* self, int16_t num_1, uint16_t num_2)
{
	return (num_1 >> num_2) & ((int)(pow(2,16-num_2)-1));
}
