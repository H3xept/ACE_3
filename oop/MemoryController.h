/**
* Filename: MemoryController.h
* Class: MemoryController
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-11
* 
* Description: Proxy level between memory and control unit
*/

#ifndef __MemoryController__
#define __MemoryController__

typedef struct _MemoryController {
	Object super;
} MemoryController;

extern const void * MemoryController_Class_Descriptor;

// Public class methods for MemoryController
// ...

// Public instance methods for MemoryController
int16_t MemoryController_Word_At_Address(MemoryController* self, int16_t addr);
void MemoryController_Set_Word_At_Address(MemoryController* self, int16_t addr, int16_t word);
void MemoryController_Clear_Memory(MemoryController* self);
void MemoryController_Load_Memory_From_Ptr(MemoryController* self, void* ptr, size_t size);

#endif
