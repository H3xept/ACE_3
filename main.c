#include <stdio.h>
#include <stdlib.h>
#include "./oop/umbrella.h"
#include "./oop/protocols/MemoryDelegate.h"

int main(int argc, char const *argv[])
{
	MemoryController* mc = alloc_init(MemoryController_Class_Descriptor);
	struct MemoryDelegate* delegate = mc->memoryDelegateVptr;
	delegate->MemoryDelegate_Clear_Memory(delegate);
}
