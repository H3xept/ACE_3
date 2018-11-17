/**
* Filename: Queue.h
* Class: Queue
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-16
* 
* Description: a queue
*/

#ifndef __Queue__
#define __Queue__

typedef struct _Queue {
	Object super;
	int cap;
	int front;
	int rear;
	int els;
	word_t* Q;
} Queue;

extern const void * Queue_Class_Descriptor;

// Public class methods for Queue
// ...

// Public instance methods for Queue
void Queue_Enqueue(Queue* self, int element);
word_t Queue_Dequeue(Queue* self);
word_t Queue_Front(Queue* self);
word_t Queue_Size(Queue* self);
bool Queue_Is_Empty(Queue* self);

#endif
