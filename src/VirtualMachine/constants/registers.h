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

#ifndef __REGISTERS__
#define __REGISTERS__

#include "var_word_size.h"
#include <ctype.h>

typedef struct {
	uword_t PC:12;
	uword_t IR:16;
	uword_t RA:12;
	uword_t SP:12;
	uword_t FP:12;
	uword_t T1:16;
	uword_t T2:16;
	uword_t T3:16;
	uword_t T4:16;
	uword_t S1:16;
	uword_t S2:16;
	uword_t S3:16;
	uword_t S4:16;
	uword_t S5:16;
	uword_t PR:16;
} Registers;

#endif
