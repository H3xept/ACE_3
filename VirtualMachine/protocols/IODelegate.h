#ifndef __IODELEGATE__
#define __IODELEGATE__

#include <ctype.h>
#include "../constants/var_word_size.h"

struct IODelegate {
	void* delegateObject;
	uword_t 	(*IODelegate_Get_Word_From_Input_Queue)(struct IODelegate * delegate);
	void 	(*IODelegate_Put_Word_To_Output_Queue)(struct IODelegate * delegate, uword_t word, uint8_t print);
};

#endif
