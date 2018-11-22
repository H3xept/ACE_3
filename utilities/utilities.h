#ifndef __UTILITIES__
#define __UTILITIES__ 
#define __DEBUG__
#define __DELEGATES__
#define __CONTROL_UNIT__
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

#define MIN(X,Y) (X < Y) ? X : Y
#define MAX(X,Y) (X > Y) ? X : Y

int16_t unsigned_to_signed(uint16_t word);
uint16_t signed_to_unsigned(int16_t word);

#endif
