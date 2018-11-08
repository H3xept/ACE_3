#ifndef __STRING__
#define __STRING__

typedef struct _String {
	Object super;
	char* text;
	char* description;
} String;

// METHODS
char* Get_Text(String* self);
void Set_Text(String* self, char* text);

extern const void * String_Class_Descriptor;

#endif
