#ifndef __FLAGDELEGATE__
#define __FLAGDELEGATE__

#include <ctype.h>

typedef enum {
	k_Status_Flag_Halt,
	k_Status_Flag_Overflow,
	k_Status_Flag_Input,
	k_Status_Flag_Exit_Codes
} k_Status_Flag;

typedef struct {
	void 	(*FlagDelegate_Set_Flag)(FlagDelegate * delegate, k_Status_Flag flag, uint8_t value);
} FlagDelegate;

#endif
