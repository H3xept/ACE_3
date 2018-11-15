/**
* Filename: ALU.h
* Class: ALU
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-12
* 
* Description: the place where arithmetic and logical operations take place
*/

#ifndef __ALU__
#define __ALU__

typedef struct {
	unsigned int halt:1;
	unsigned int overflow:1;
	unsigned int input:1;
	unsigned int exit_codes:2;
} FlagRegister;

typedef struct _ALU {
	Object super;
	FlagRegister flags;
} ALU;

extern const void * ALU_Class_Descriptor;

// Public class methods for ALU
// ...

// Public instance methods for ALU
int16_t ALU_Add(ALU* self, int16_t num_1, int16_t num_2);
int16_t ALU_Multiply(ALU* self, int16_t num_1, int16_t num_2);
int16_t ALU_Divide(ALU* self, int16_t num_1, int16_t num_2);
int16_t ALU_Bitwise_And(int16_t num_1, int16_t num_2);
int16_t ALU_Bitwise_Or(ALU* self, int16_t num_1, int16_t num_2);
int16_t ALU_Bitwise_Not(ALU* self, int16_t num);
int16_t ALU_Shift_Left_Logical(ALU* self, int16_t num_1, uint16_t num_2);
int16_t ALU_Shift_Right_Logical(ALU* self, int16_t num_1, uint16_t num_2);

#endif
