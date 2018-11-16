#ifndef __FLAGDELEGATE__
#define __FLAGDELEGATE__

#include <ctype.h>
#include "../constants/var_word_size.h"

typedef enum {
	k_Status_Flag_Halt,
	k_Status_Flag_Overflow,
	k_Status_Flag_Input,
	k_Status_Flag_Exit_Code
} k_Status_Flag;

struct FlagDelegate{
	void 	(*FlagDelegate_Set_Flag)(struct FlagDelegate * delegate, k_Status_Flag flag, uint8_t value);
	uword_t (*FlagDelegate_Get_Flags_As_Word)(struct FlagDelegate * delegate);
	uint8_t (*FlagDelegate_Read_Flag)(struct FlagDelegate * delegate, k_Status_Flag flag);
};

#endif
