/**************************************************************************
 * Assessment Title:
 * ACE 3 (TBTTBSQSA)
 *
 * Number of Submitted C Files: 
 * 19 (51 total files including .h, .py, .asm and .md)
 * 
 * Date: 
 * 25/11/2018
 * 
 * Authors: 
 *	1. Leonardo Cascianelli, Reg no: 201710666
 *	2. Rory Brown, Reg no: 201717976
 *	3. Ewan Skene, Reg no: 201717413
 * 
 *
 *	Statement: We confirm that this submission is all our own work.
 *
 *  (Signed)_Leonardo_Cascianelli________________(Leonardo Cascianelli)
 *	
 * 	(Signed)_Rory_Brown__________________________(Rory Brown)
 *	
 *	(Signed)_Ewan_Skene__________________________(Ewan Skene)
 *
 **************************************************************************/

/**
* Filename: Queue.h
* Class: Queue
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-16
* 
* Description: A queue of uint16_t
*/

#ifndef __Queue__
#define __Queue__

#include <stdint.h>

typedef struct _Queue {
	Object super;
	int cap;
	int front;
	int rear;
	int els;
	uint16_t* Q;
} Queue;

extern const void * Queue_Class_Descriptor;

// Public instance methods for Queue
void Queue_Enqueue(Queue* self, uint16_t element);
uint16_t Queue_Dequeue(Queue* self);
uint16_t Queue_Front(Queue* self);
uint16_t Queue_Size(Queue* self);
uint16_t Queue_Is_Empty(Queue* self);

#endif
