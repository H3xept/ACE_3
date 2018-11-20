#include <stdio.h>
#include <stdlib.h>
#include "./oop/umbrella.h"
#include "VirtualMachine/Program.h"
#include "VirtualMachine/VirtualMachine.h"

int main(int argc, char const *argv[])
{
	VirtualMachine* vm = alloc_init(VirtualMachine_Class_Descriptor);
	return 0;
}
