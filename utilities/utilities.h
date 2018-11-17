#ifndef __UTILITIES__
#define __UTILITIES__ 
#define __DEBUG__
#include "colors.h"
#include <stdint.h>
#include "../oop/constants/var_word_size.h"

#define _warn(S,...) 	y_printf("[WARNING] ",NULL); \
						y_printf(S, __VA_ARGS__); \
						puts("");	
						
#define _err(S, ...) 	r_printf("[ERROR] ",NULL); \
						r_printf(S,__VA_ARGS__); \
						r_printf("\n{File:%s|Line:%d|Function:%s}\n",__FILE__,__LINE__,__func__); \
						exit(-1);

#ifdef __DEBUG__
#define _info(S, ...) 	g_printf("[INFO] ",NULL); \
						g_printf(S, __VA_ARGS__); \
						puts("");
#else
#define _info(S, ...)
#endif

#define MIN(X,Y) (X < Y) ? X : Y
#define MAX(X,Y) (X > Y) ? X : Y

inline word_t unsigned_to_signed(uword_t word);
inline uword_t signed_to_unsigned(word_t word);

#endif
