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
#include "./constants/arch_const.h"
#include "./constants/var_word_size.h"
#include "../oop/umbrella.h"
#include "ALU.h"
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

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: ALU constructor.
* @param self: A reference to the current instance of ALU
* @param args: Takes no variadic args.
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

// Public instance methods for ALU

/**
* @brief: returns sum of two 16bit ints.
* @param self: A reference to the current instance of ALU
* @param num_1: first number in sum
* @param num_2: second number in sum
* @return int16/32_t: result of addition.
*/
word_t ALU_Add(ALU* self, word_t num_1, word_t num_2)
{
	word_t ret = 0;
	if ((num_1 > 0 && num_2 > WORD_MAX - num_1) || 
		(num_1 < 0 && num_2 < WORD_MIN - num_1)) {
		_warn("Overflow detected!", NULL);
		uword_t tnum_1 = 1+UWORD_MAX+num_1;
		uword_t tnum_2 = 1+UWORD_MAX+num_2;
		ret = 0 | (tnum_2+tnum_1);
		self->flags.overflow = 1;
	}
	else{
		self->flags.overflow = 0;
		ret = num_1+num_2;
	}
	return ret;
}

/**
* @brief: returns product of two 16bit ints.
* @param self: A reference to the current instance of ALU
* @param num_1: first number in multiplication
* @param num_2: second number in multiplication
* @return int16/32_t: result of multiplication.
*/
word_t ALU_Multiply(ALU* self, word_t num_1, word_t num_2)
{
	unsigned int overflow = 0;
	word_t ret = 0;

	if (num_1 > 0)
	{
		if (num_2 > 0 && num_1 > WORD_MAX/num_2){
			overflow = 1;
		} else if(num_1 < WORD_MIN/num_2) {
			overflow = 1;
		}
	} else {
		if (num_2 > 0 && num_2 < WORD_MIN/num_1) {
			overflow = 1;
		} else if (num_1 != 0 && num_2 < WORD_MAX/num_1) {
			overflow = 1;
		}
	} self->flags.overflow = overflow;

	if (overflow) {
		_warn("Overflow detected!", NULL);
		uword_t tnum_1 = 1+UWORD_MAX+num_1;
		uword_t tnum_2 = 1+UWORD_MAX+num_2;
		ret = 0 | tnum_2*tnum_1;
	} else {
		ret = num_1*num_2;
	}
	return ret;
}

/**
* @brief: returns result of dividing of two 16bit ints.
* @param self: A reference to the current instance of ALU
* @param num_1: number to be divided
* @param num_2: divides by this argument
* @return int16/32_t: result of division.
*/
word_t ALU_Divide(ALU* self, word_t num_1, word_t num_2)
{
	if (num_2 == 0) 
		_err("Division by zero! Aborting...", NULL);
	if (num_1 == WORD_MIN && num_2 == -1) {
		_warn("Overflow detected!", NULL);
		self->flags.overflow = 1;
		return num_1;
	} self->flags.overflow = 0;
	return num_1/num_2;
}

/**
* @brief: bitwise AND of two 16bit ints.
* @param self: A reference to the current instance of ALU
* @param num_1: number to be anded with 2nd arg
* @param num_2: the second argument
* @return int16/32_t: result of ANDing.
*/
word_t ALU_Bitwise_And(ALU* self, word_t num_1, word_t num_2)
{
	return num_1 & num_2;
}


/**
* @brief: bitwise OR of two 16bit ints.
* @param self: A reference to the current instance of ALU
* @param num_1: number to be anded with 2nd arg
* @param num_2: the second argument
* @return int16/32_t: result of ORing.
*/
word_t ALU_Bitwise_Or(ALU* self, word_t num_1, word_t num_2)
{
	return num_1 | num_2;
}

/**
* @brief: bitwise NOT of one 16bit int.
* @param self: A reference to the current instance of ALU
* @param num_1: number to be anded with 2nd arg
* @return int16/32_t: result of NOTing.
*/
word_t ALU_Bitwise_Not(ALU* self, word_t num)
{
	return ~num;
}

/**
* @brief: logical shift left of arg1 by arg2.
* @param self: A reference to the current instance of ALU
* @param num_1: number shifted
* @param num_2: places shifted
* @return int16/32_t: result of shifting num_1 by num_2.
*/
word_t ALU_Shift_Left_Logical(ALU* self, word_t num_1, uword_t num_2)
{
	return num_1 << num_2;
}

/**
* @brief: logical right left of arg1 by arg2.
* @param self: A reference to the current instance of ALU
* @param num_1: number shifted
* @param num_2: places shifted
* @return int16/32_t: result of shifting num_1 by num_2.
*/
word_t ALU_Shift_Right_Logical(ALU* self, word_t num_1, uword_t num_2)
{
	return (num_1 >> num_2) & ((int)(pow(2,16-num_2)-1));
}
