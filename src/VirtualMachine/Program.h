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
* Filename: Program.h
* Class: Program
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-20
* 
* Description: A program for the ACE3 Machine
*/

#ifndef __Program__
#define __Program__

#include "./constants/var_word_size.h"

typedef struct _Program {
	Object super;
	uword_t* source;
	size_t size; 
} Program;

extern const void * Program_Class_Descriptor;

// Public class methods for Program

Program* Program_With_File(const char* filename);
Program* Program_With_Buffer(uword_t* buffer, size_t len);

#endif
