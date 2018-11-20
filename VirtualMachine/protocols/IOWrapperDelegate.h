#ifndef __IOWRAPPER_DELEGATE__
#define __IOWRAPPER_DELEGATE__

#include "../../oop/umbrella.h"
#include "../constants/var_word_size.h"

struct IOWrapperDelegate {
	void* delegateObject;
	uword_t* 	(*IOWrapperDelegate_Input)(struct IOWrapperDelegate * delegate);
	void 	(*IOWrapperDelegate_Output)(struct IOWrapperDelegate * delegate, Queue* output_queue);
};

#endif
