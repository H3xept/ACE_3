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

#ifndef __ARCH_CONSTS__
#define __ARCH_CONSTS__

#define WORD_SIZE 16
#define MIN_BITS_ADDRESSABLE 16
#define ADDR_LENGTH 12
#define OPCODE_LENGTH 4
#define REGISTER_ADDR_LENGTH 4
#define WORDS_IN_MEM (int)(((pow(2,ADDR_LENGTH))*MIN_BITS_ADDRESSABLE)/WORD_SIZE)
#define TOTAL_MEM WORD_SIZE*WORDS_IN_MEM
// Arbitrary: we just assumed that the stack will be big enough being 1/4 of total memory.
#define MAX_PROG_SIZE ((3.0/4.0)*TOTAL_MEM) 
#endif
