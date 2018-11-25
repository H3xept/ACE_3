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
* Filename: VMLogger.h
* Class: VMLogger
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-25
* 
* Description: The logger for the virtual machine
*/

#ifndef __VMLogger__
#define __VMLogger__

#include <stdint.h>
#include "../Disassembler/Disassembler.h"
#include "./constants/registers.h"

typedef struct _VMLogger {
	Object super;
	FILE* __trace_file_obj;
	char* __reg_file;
	char* __trace_file;
	char* __memdump_file;
} VMLogger;

extern const void * VMLogger_Class_Descriptor;

// Public instance methods for VMLogger
void VMLogger_Dump_Memory(VMLogger* self, const void* memory);
void VMLogger_Append_New_Binary_Instruction(VMLogger* self, uword_t instruction);
void VMLogger_Close_Trace_File(VMLogger* self);
void VMLogger_Dump_Registers(VMLogger* self, Registers* registers);
#endif
