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

#ifndef __FLAGDELEGATE__
#define __FLAGDELEGATE__

#include <ctype.h>
#include "../constants/var_word_size.h"

typedef enum {
	k_Exit_Code_Halt,
	k_Exit_Code_Segfault,
	k_Exit_Code_Illegal_Register_Access
} k_Exit_Code;

typedef enum {
	k_Status_Flag_Halt,
	k_Status_Flag_Overflow,
	k_Status_Flag_Input,
	k_Status_Flag_Exit_Code
} k_Status_Flag;

struct FlagDelegate{
	void* delegateObject;
	void 	(*FlagDelegate_Set_Flag)(struct FlagDelegate * delegate, k_Status_Flag flag, uint8_t value);
	uword_t (*FlagDelegate_Get_Flags_As_Word)(struct FlagDelegate * delegate);
	uint8_t (*FlagDelegate_Read_Flag)(struct FlagDelegate * delegate, k_Status_Flag flag);
};

#endif
