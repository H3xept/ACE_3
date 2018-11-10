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
// --

static struct ObjectVTbl 	vtbl = {
								&_Object_Ctor,
								&_Object_Dtor,
								&_Object_Type_Descriptor,
								&_Object_Descriptor
							};

static struct Class_Descriptor _String_Class_Descriptor = {
	sizeof(String),
	&vtbl
};
const void * String_Class_Descriptor = &_String_Class_Descriptor;

// VIRTUAL METHODS (PRIVATE)

static Object* _Object_Ctor(Object * self, va_list args)
{
	String* _self = (String*)self;

	char* text = va_arg(args, char*);
	_self->text = calloc(1, strlen(text)+1);
	memcpy(_self->text, text, strlen(text));

	_self->description = __Compute_Description(_self);
	return self;
}

static Object* _Object_Dtor(Object * self)
{
	String* _self = (String*)self;
	if (_self->text)
		free(_self->text); _self->text = 0;
	if (_self->description)
		free(_self->description); _self->description = 0;
	return self;
}

static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

static const char* const _Object_Descriptor(Object * self)
{
	String* _self = (String*)self;
	if (!(_self->description))
		_self->description = __Compute_Description(_self);
	return _self->description;
}

// ------------- END BOILERPLATE

static const char* const 	DESCRIPTOR_PREFIX = "<String:";
static const char* const 	DESCRIPTOR_SUFFIX = ">";

// PRIVATE FUNCTIONS
static char* make_description(const char* const prefix, const char* const suffix, const char* const str);

// PRIVATE METHODS 
static void __Invalidate_Description(String* self);
static char* __Compute_Description(String* self);

// NON-VIRTUAL METHODS

char* Get_Text(String * self)
{
	return self->text;
}

void Set_Text(String * self, char* text)
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
	return make_description(DESCRIPTOR_PREFIX, DESCRIPTOR_SUFFIX, Get_Text(self));
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
