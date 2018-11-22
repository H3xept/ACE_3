#include <stdio.h>
#include <stdlib.h>
#include "./oop/umbrella.h"
#include "VirtualMachine/Program.h"
#include "VirtualMachine/VirtualMachine.h"

int main(int argc, char const *argv[])
{
	Program* program = Program_With_File("out3");
	VirtualMachine* vm = alloc_init(VirtualMachine_Class_Descriptor);
	Virtual_Machine_Run(vm, program);
	return 0;
}
