#ifndef __FLAGDELEGATE__
#define __FLAGDELEGATE__

#include <ctype.h>

typedef enum {
	k_Exit_Code_Halt
	k_Exit_Code_Segfault,
	k_Exit_Code_Illegal_Register_Access;
} k_Exit_Code;

typedef enum {
	k_Status_Flag_Halt,
	k_Status_Flag_Overflow,
	k_Status_Flag_Input,
	k_Status_Flag_Exit_Code
} k_Status_Flag;

typedef struct {
	void 	(*FlagDelegate_Set_Flag)(FlagDelegate * delegate, k_Status_Flag flag, uint8_t value);
} FlagDelegate;

#endif
