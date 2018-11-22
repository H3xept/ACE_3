/**
* Filename: String.h
* Class: String
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-16
* 
* Description: a String class
*/

#ifndef __STRING__
#define __STRING__

typedef struct _String {
	Object super;
	char* text;
	char* description;
} String;

// METHODS
char* String_Get_Text(String * self);
void String_Set_Text(String* self, char* text);

extern const void * String_Class_Descriptor;

#endif
