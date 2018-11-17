#ifndef __VAR_WORD_SIZE__
#define __VAR_WORD_SIZE__

#include "arch_const.h"

#if WORD_SIZE == 16
typedef int16_t word_t;
typedef uint16_t uword_t;
	#define WORD_MAX 32767
	#define WORD_MIN -32768
	#define UWORD_MAX 65535
#else
typedef int32_t word_t;
typedef uint32_t uword_t;
	#define WORD_MAX 2147483647
	#define WORD_MIN -2147483648
	#define UWORD_MAX 4294967295
#endif

#endif
