#ifndef __OOP__
#define __OOP__

#include <stddef.h>
#include "Object.h"
#include "Class_Descriptor.h"

#define unsafe_obj_desc(obj) Object_Descriptor((Object*)obj)

void * alloc_init(const struct Class_Descriptor* descriptor, ...);
void dealloc(void * ptr);

#endif
