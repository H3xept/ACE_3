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

typedef struct _ALU {
	Object super;
} ALU;

extern const void * ALU_Class_Descriptor;

// Public class methods for ALU
// ...

// Public instance methods for ALU
bool SKC(ALU* self, int16_t num);
int16_t ADD(ALU* self, int16_t num_1, int16_t num_2);
int16_t MUL(ALU* self, int16_t num_1, int16_t num_2);
int16_t DIV(ALU* self, int16_t num_1, int16_t num_2);
int16_t AND(ALU* self, int16_t num_1, int16_t num_2);
int16_t OR(ALU* self, int16_t num_1, int16_t num_2);
int16_t NOT(ALU* self, int16_t num);
int16_t SHL(ALU* self, int16_t num_1, int16_t num_2);
int16_t SHR(ALU* self, int16_t num_1, int16_t num_2);

#endif
