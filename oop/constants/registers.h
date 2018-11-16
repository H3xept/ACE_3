// 0000 / PC Program Counter (12 bits)
// 0001 / IR Instruction Register (16 bits)
// 0010 / MAR Memory Address Register (12 bits)
// 0011 / MBR Memory Buffer Register (16 bits)
// 0100 / RA Return Address Register (12 bits)
// 0101 / INREG Input Register (16 bits)
// 0110 / OUTREG Output Register (16 bits)
// 0111 / T1 Temporary Register 1 (16 bits)
// 1001 / T2 Temporary Register 2 (16 bits)
// 1010 / S1 Saved Register 1 (16 bits)
// 1011 / S2 Saved Register 2 (16 bits)
// 1100 / SP Stack Pointer (12 bits)
// 1101 / FP Frame Pointer (12 bits)
// 1110 / FR Flag Register (2 bits)
// 1111 / PR Pseudoinstruction Register (16 bits)

##ifndef __REGISTERS__
#define __REGISTERS__

#include "var_word_size.h"
#include <ctype.h>

typedef struct {
	uword_t PC;
	uword_t IR;
	uword_t RA;
	uword_t INREG;
	uword_t OUTREG;
	uword_t T1;
	uword_t T2;
	uword_t S1;
	uword_t S2;
	uword_t SP;
	uword_t FP;
	uword_t FR;
	uword_t PR;
} Registers;

#endif