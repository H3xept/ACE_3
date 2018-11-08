#ifndef __UTILITIES__
#define __UTILITIES__ 
#include "colors.h"

#define _warn(S,...) 	{	y_printf("[WARNING] ",NULL); \
							y_printf(S, __VA_ARGS__); \
							puts("\n");	\
						}
#define _err(S, ...) 	{	r_printf("[ERROR] ",NULL); \
							r_printf(S,__VA_ARGS__); \
							r_printf("\n{File:%s|Line:%d|Function:%s}\n",__FILE__,__LINE__,__func__); \
							exit(-1);\
						}
#define _info(S, ...) 	{	g_printf("[INFO] ",NULL); \
							g_printf(S, __VA_ARGS__); \
							puts("\n");	\
						}

#endif
