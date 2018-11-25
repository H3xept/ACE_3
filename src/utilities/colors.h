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
* Filename: colors.h
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-16
* 
* Description: defines set of colours and colour printing macros
*/


#define a_RED  		"\x1B[31m"
#define a_GREEN  	"\x1B[32m"
#define a_YELLOW  	"\x1B[33m"
#define a_BLUE  	"\x1B[34m"
#define a_MAGENTA  	"\x1B[35m"
#define a_CYAN  	"\x1B[36m"
#define RESET_COLOR "\x1b[0m"

#define r_printf(format, ...) cprintf(a_RED, format, __VA_ARGS__)
#define g_printf(format, ...) cprintf(a_GREEN, format, __VA_ARGS__)
#define y_printf(format, ...) cprintf(a_YELLOW, format, __VA_ARGS__)
#define b_printf(format, ...) cprintf(a_BLUE, format, __VA_ARGS__)
#define m_printf(format, ...) cprintf(a_MAGENTA, format, __VA_ARGS__)
#define c_printf(format, ...) cprintf(a_CYAN, format, __VA_ARGS__)

void cprintf(char* color_code, char* format, ...);
