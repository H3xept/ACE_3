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
* Filename: colors.c
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-16
* 
* Description: function to print in custom colours
*/

#include <stdarg.h>
#include <stdio.h>
#include "colors.h"

void cprintf(char* color_code, char* format, ...)
{
	va_list args; 
	va_start(args,format);
	printf("%s",color_code);
	vprintf(format,args);
	printf("%s",RESET_COLOR);
	va_end(args);
}
