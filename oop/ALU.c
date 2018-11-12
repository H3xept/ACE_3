/**
* Filename: ALU.c
* Class: ALU
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-12
* 
* Description: the place where arithmetic and logical operations take place
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Object.h"
#include "OOP.h"
#include "../utilities/utilities.h"
#include "ALU.h"

/// The type string of ALU
static const char* const 	type_string = "ALU";

/// Private overrides for 'Object' virtual methods (signature)
static Object* 			 	_Object_Ctor(Object * self, va_list args);
static Object* 			 	_Object_Dtor(Object * self);
static const char* const 	_Object_Type_Descriptor(Object * _self);
static const char* const 	_Object_Descriptor(Object * self);

/// Function binding for virtual methods table
static struct ObjectVTbl 	vtbl = {
								&_Object_Ctor,
								&_Object_Dtor,
								&_Object_Type_Descriptor,
								&_Object_Descriptor
							};

/// Class descriptor structure to instantiate ALU
static struct Class_Descriptor _ALU_Class_Descriptor = {
	sizeof(ALU),
	&vtbl
};
const void * ALU_Class_Descriptor = &_ALU_Class_Descriptor;

// private fields
// ...

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: ALU constructor.
* @param self: A reference to the current instance of ALU
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	// Downcast to ALU
	ALU* _self = (ALU*)self;
	_warn("Class ALU does not respond to %s",__func__);
	assert(0);
	return NULL;
}

/**
* @brief: ALU destructor.
* @param self: A reference to the current instance of ALU
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	// Downcast to ALU
	ALU* _self = (ALU*)self;
	_warn("Class ALU does not respond to %s",__func__);
	assert(0);
	return NULL;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of ALU
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of ALU
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	return "<theALU>";
}

// Private class methods for ALU
// ...

// Private instance methods for ALU
// ...

// Public class methods for ALU
// ...

// Public instance methods for ALU
// ...

//returns true if value given was greater than 0
bool SKC(int16_t num){
	return (num > 0);
}

//returns sum of two 16bit ints
int16_t ADD(int16_t num_1, int16_t num_2, int16_t* status_reg){
	int16_t ans = num_1 + num_2;
	if((num_1 >> 15 == num_2 >> 15) & (num_1 >> 15 != ans >> 15)) 
		status_reg = OR(status_reg, 0b10);
	return ans;
}

//returns product of two 16bit ints
//DETECTING OVERFLOW? ALSO SIZE OF PRODUCT TOO BIG?(I think this works...)
int16_t MUL(int16_t num_1, int16_t num_2, int16_t* status_reg){
	int16_t ans = num_1 * num_2;
	if(((num_1 >> 15 == num_2 >> 15) & (num_1 >> 15 != ans >> 15)) | ((num_1 >> 15 != num_2 >> 15) &  ((ans >> 15) == 0)))
		*status_reg = OR(*status_reg, 0b10);
	return ans;
}

//returns result of first argument divided by second argument
int16_t DIV(int16_t num_1, int16_t num_2){
	return num_1 / num_2;
}

//bitwise AND of two 16bit ints 
int16_t AND(int16_t num_1, int16_t num_2){
	return num_1 & num_2;
}

//bitwise OR of two 16bit ints 
int16_t OR(int16_t num_1, int16_t num_2){
	return num_1 | num_2;
}

int16_t NOT(int16_t num){
	return ~num;
}

//logical left-shift on argument 1 (argument 2 specifies displacement)
int16_t SHL(int16_t num_1, int16_t num_2){
	return num_1 << num_2;
}

//logical right-shift on argument 1 (argument 2 specifies displacement)
int16_t SHR(int16_t num_1, int16_t num_2){
	return num_1 >> num_2;
}





