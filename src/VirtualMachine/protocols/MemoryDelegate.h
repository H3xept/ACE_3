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

#ifndef __MemoryDelegate__
#define __MemoryDelegate__

#include "../constants/var_word_size.h"

struct MemoryDelegate {
	void* delegateObject;
	uword_t (*MemoryDelegate_Word_At_Address)(struct MemoryDelegate * delegate, uword_t addr);
	void (*MemoryDelegate_Set_Word_At_Address)(struct MemoryDelegate * delegate, uword_t addr, uword_t word);
	void (*MemoryDelegate_Clear_Memory)(struct MemoryDelegate * delegate);
	void (*MemoryDelegate_Load_Memory_From_Ptr)(struct MemoryDelegate * delegate, void* ptr, size_t size);
	const void* (*MemoryDelegate_Dump_Readonly_Memory)(struct MemoryDelegate* delegate);
};

#endif
