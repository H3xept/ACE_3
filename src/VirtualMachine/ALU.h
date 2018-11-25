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
#include "../oop/umbrella.h"

typedef struct _ALU {
	Object super;
	FlagRegister flags;
} ALU;

extern const void * ALU_Class_Descriptor;

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
