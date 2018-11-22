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

// Public instance methods for Program
// ...

#endif
