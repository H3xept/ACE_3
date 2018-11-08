#ifndef __OOP__
#define __OOP__

#include <stddef.h>
#include "Object.h"

#define unsafe_obj_desc(obj) Object_Descriptor((Object*)obj)

struct Class_Descriptor {
	size_t size;
	void * vptr;
};

void * alloc_init(const struct Class_Descriptor* descriptor, ...);
void dealloc(void * ptr);

#endif
