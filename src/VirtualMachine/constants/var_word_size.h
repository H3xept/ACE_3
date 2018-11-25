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

#ifndef __VAR_WORD_SIZE__
#define __VAR_WORD_SIZE__

#include "arch_const.h"
#include <stdint.h>

#if WORD_SIZE == 16

typedef int16_t word_t;
typedef uint16_t uword_t;
#define WORD_MAX 32767
#define WORD_MIN -32768
#define UWORD_MAX 65535
#else
typedef int32_t word_t;
typedef uint32_t uword_t;
#define WORD_MAX 2147483647
#define WORD_MIN -2147483648
#define UWORD_MAX 4294967295
#endif
#endif
