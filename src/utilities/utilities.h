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
* Filename: utilities.h
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-16
* 
* Description: utility macros
*/


#ifndef __UTILITIES__
#define __UTILITIES__ 
// #define __DEBUG__
// #define __DELEGATES__
// #define __CONTROL_UNIT__
#include "colors.h"
#include <stdint.h>

#define _warn(S,...) 	{ y_printf("[WARNING] ",NULL); \
						y_printf(S, __VA_ARGS__); \
						puts(""); }
						
#define _err(S, ...) 	{ r_printf("[ERROR] ",NULL); \
						r_printf(S,__VA_ARGS__); \
						r_printf("\n{File:%s|Line:%d|Function:%s}\n",__FILE__,__LINE__,__func__); \
						exit(-1); }

#ifdef __CONTROL_UNIT__
#define _controlInfo(S, ...) { m_printf("[INFO] ",NULL); \
						m_printf(S, __VA_ARGS__); \
						puts(""); }
#else
#define _controlInfo(S, ...)
#endif 

#ifdef __DELEGATES__
#define _delegCall()	b_printf("[DELEGATE] %s Received delegate call -> %s\n",__FILE__, __func__)
#else 
#define _delegCall()
#endif 

#ifdef __DEBUG__
#define _info(S, ...) 	{ g_printf("[INFO] ",NULL); \
						g_printf(S, __VA_ARGS__); \
						puts(""); }
#else
#define _info(S, ...)
#endif

#define _pinfo(S, ...) 	{ g_printf("[INFO] ",NULL); \
						g_printf(S, __VA_ARGS__); \
						puts(""); }

#define MIN(X,Y) (X < Y) ? X : Y
#define MAX(X,Y) (X > Y) ? X : Y

int16_t unsigned_to_signed(uint16_t word);
uint16_t signed_to_unsigned(int16_t word);

#endif
