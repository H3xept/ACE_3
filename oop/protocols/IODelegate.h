#ifndef __IODELEGATE__
#define __IODELEGATE__

#include <ctype.h>
#include "../constants/var_word_size.h"

typedef struct {
	word_t 	(*IODelegate_Get_Word_From_Input_Queue)(IODelegate * delegate);
	void 	(*IODelegate_Put_Word_To_Output_Queue)(IODelegate * delegate, word_t word, uint8_t print);
} IODelegate;

#endif
