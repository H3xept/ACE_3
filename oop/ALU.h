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
bool SKC(int16_t num);
int16_t ADD(int16_t num_1, int16_t num_2);
int16_t MUL(int16_t num_1, int16_t num_2);
int16_t DIV(int16_t num_1, int16_t num_2);
int16_t AND(int16_t num_1, int16_t num_2);
int16_t OR(int16_t num_1, int16_t num_2);
int16_t NOT(int16_t num);
int16_t SHL(int16_t num_1, int16_t num_2);
int16_t SHR(int16_t num_1, int16_t num_2);

#endif
