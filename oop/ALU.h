/**
* Filename: ALU.h
* Class: ALU
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-12
* 
* Description: the place where arithmetic and logical operations take place
*/

#ifndef __ALU__
#define __ALU__

#include "./constants/flag_register.h"
#include "./constants/var_word_size.h"

typedef struct _ALU {
	Object super;
	FlagRegister flags;
} ALU;

extern const void * ALU_Class_Descriptor;

// Public class methods for ALU
// ...

// Public instance methods for ALU
word_t ALU_Add(ALU* self, word_t num_1, word_t num_2);
word_t ALU_Multiply(ALU* self, word_t num_1, word_t num_2);
word_t ALU_Divide(ALU* self, word_t num_1, word_t num_2);
word_t ALU_Bitwise_And(ALU* self, word_t num_1, word_t num_2);
word_t ALU_Bitwise_Or(ALU* self, word_t num_1, word_t num_2);
word_t ALU_Bitwise_Not(ALU* self, word_t num);
word_t ALU_Shift_Left_Logical(ALU* self, word_t num_1, uword_t num_2);
word_t ALU_Shift_Right_Logical(ALU* self, word_t num_1, uword_t num_2);

#endif
