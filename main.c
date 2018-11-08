#include <stdio.h>
#include <stdlib.h>
#include "./oop/umbrella.h"

int main(int argc, char const *argv[])
{
	String* str = alloc_init(String_Class_Descriptor, "Test");
	Set_Text(str, "Ufo Porno");
	printf("%s\n",unsafe_obj_desc(str));
	printf("%s\n",Get_Text(str));
	_err("Test",NULL);
	return 0;
}
