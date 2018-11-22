/**
* Filename: String.c
* Class: String
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-16
* 
* Description: a String class
*/


#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Object.h"
#include "String.h"
#include "OOP.h"
#include "../utilities/utilities.h"

static const char* const 	type_string = "String";

// INTERFACE - @OVERRIDE VIRTUAL METHODS
static Object* 			 	_Object_Ctor(Object * self, va_list args);
static Object* 			 	_Object_Dtor(Object * self);
static const char* const 	_Object_Type_Descriptor(Object * _self);
static const char* const 	_Object_Descriptor(Object * self);
static unsigned int 		_Object_Equals(Object* self, Object* obj);
// --

static struct ObjectVTbl 	vtbl = {
								&_Object_Ctor,
								&_Object_Dtor,
								&_Object_Type_Descriptor,
								&_Object_Descriptor,
								&_Object_Equals
							};

static struct Class_Descriptor _String_Class_Descriptor = {
	sizeof(String),
	&vtbl
};
const void * String_Class_Descriptor = &_String_Class_Descriptor;

static const char* const 	DESCRIPTOR_PREFIX = "<String:";
static const char* const 	DESCRIPTOR_SUFFIX = ">";

// PRIVATE FUNCTIONS
static char* make_description(const char* const prefix, const char* const suffix, const char* const str);

// PRIVATE METHODS 
static void __Invalidate_Description(String* self);
static char* __Compute_Description(String* self);
static char* __Compute_Description(String* self);

// VIRTUAL METHODS (PRIVATE)

/**
* @brief: String constructor.
* @param self: A reference to the current instance of String
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	String* _self = (String*)self;

	char* text = va_arg(args, char*);
	_self->text = calloc(1, strlen(text)+1);
	memcpy(_self->text, text, strlen(text));

	_self->description = __Compute_Description(_self);
	return self;
}

/**
* @brief: String destructor.
* @param self: A reference to the current instance of String
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	String* _self = (String*)self;
	if (_self->text)
		free(_self->text); _self->text = 0;
	if (_self->description)
		free(_self->description); _self->description = 0;
	return self;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of String
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of String
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	String* _self = (String*)self;
	if (!(_self->description))
		_self->description = __Compute_Description(_self);
	return _self->description;
}

/**
* @brief: Returns 0 if object is not the same instance as another.
* @param self: A reference to the current instance of String.
* @param obj: A reference to the another instance of String.
* @return: unsigned int: 0 if not equal.
*/
static unsigned int _Object_Equals(Object* self, Object* obj)
{
	String* _self = (String*)self;
	String* _obj = (String*)obj;
	return _self->text == _obj->text;
}

// ------------- END BOILERPLATE

// NON-VIRTUAL METHODS

/**
* @brief: gets text of string.
* @param self: A reference to the current instance of String.
* @return char*: string's text.
*/
char* String_Get_Text(String * self)
{
	return self->text;
}

/**
* @brief: sets text of string.
* @param self: A reference to the current instance of String.
*/
void String_Set_Text(String * self, char* text)
{
	if (self->text)
		free(self->text);
	self->text = calloc(1,strlen(text)+1);
	memcpy(self->text, text, strlen(text));
	__Invalidate_Description(self);
}


// PRIVATE METHODS 
static void __Invalidate_Description(String* self)
{
	if (self->description)
		free(self->description); self->description = 0;
}

static char* __Compute_Description(String* self)
{
	return make_description(DESCRIPTOR_PREFIX, DESCRIPTOR_SUFFIX, String_Get_Text(self));
}

// PRIVATE FUNCTIONS
static char* make_description(const char* const prefix, const char* const suffix, const char* const str)
{
	size_t len_prefix = strlen(prefix);
	size_t len_suffix = strlen(suffix);
	size_t len_str = strlen(str);

	char* description = calloc(1, len_prefix+len_suffix+len_str+1);
	memcpy(description, prefix, len_prefix);
	memcpy(description+len_prefix, str, len_str);
	memcpy(description+len_prefix+len_str, suffix, len_suffix);

	return description;
}
