// + `0000 / PC` Program Counter (12 bits)
// + `0001 / IR` Instruction Register (16 bits)
// + `0010 / RA` Return Address Register (12 bits)
// + `0011 / SP` Stack Pointer (12 bits)
// + `0100 / FP` Frame Pointer (12 bits)
// + `0101 / T1` Temporary Register 1 (16 bits)
// + `0110 / T2` Temporary Register 2 (16 bits)
// + `0111 / T3` Temporary Register 3 (16 bits)
// + `1000 / S1` Temporary Register 4 (16 bits)
// + `1001 / S2` Saved Register 1 (16 bits)
// + `1010 / S3` Saved Register 2 (16 bits)
// + `1011 / S3` Saved Register 3 (16 bits)
// + `1100 / S3` Saved Register 4 (16 bits)
// + `1101 / S3` Saved Register 5 (16 bits)
// + `1110 / PR` Pseudoinstruction Register (16 bits)
// + `1111 / FR` Flag Register (5 bits)

##ifndef __REGISTERS__
#define __REGISTERS__

#include "var_word_size.h"
#include <ctype.h>

typedef struct {
	uword_t PC:12;
	uword_t IR:16;
	uword_t RA:12;
	uword_t SP:12;
	uword_t FP:12;
	uword_t T1:16;
	uword_t T2:16;
	uword_t T3:16;
	uword_t T4:16;
	uword_t S1:16;
	uword_t S2:16;
	uword_t S3:16;
	uword_t S4:16;
	uword_t S5:16;
	uword_t PR:16;
} Registers;

#endif