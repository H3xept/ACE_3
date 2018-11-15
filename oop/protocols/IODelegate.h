#ifndef __IODELEGATE__
#define __IODELEGATE__

#include <ctype.h>

#warning Temporary
#define WORD_SIZE 16
#if WORD_SIZE == 16
typedef int16_t word_t;
#else
typedef int32_t word_t;
#endif

typedef struct {
	word_t 	(*IODelegate_Get_Word_From_Input_Queue)(IODelegate * delegate);
	void 	(*IODelegate_Put_Word_To_Output_Queue)(IODelegate * delegate, word_t word, uint8_t print);
} IODelegate;

#endif
