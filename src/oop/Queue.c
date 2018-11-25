/**
* Filename: Queue.c
* Class: Queue
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-16
* 
* Description: a queue of uint16_t
*/

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Object.h"
#include "OOP.h"
#include "Queue.h"
#include "../utilities/utilities.h"
/// The type string of Queue
static const char* const 	type_string = "Queue";

/// Private overrides for 'Object' virtual methods (signature)
static Object* 			 	_Object_Ctor(Object * self, va_list args);
static Object* 			 	_Object_Dtor(Object * self);
static const char* const 	_Object_Type_Descriptor(Object * _self);
static const char* const 	_Object_Descriptor(Object * self);
static unsigned int 		_Object_Equals(Object* self, Object* obj);
/// Function binding for virtual methods table
static struct ObjectVTbl 	vtbl = {
								&_Object_Ctor,
								&_Object_Dtor,
								&_Object_Type_Descriptor,
								&_Object_Descriptor,
								&_Object_Equals
							};

/// Class descriptor structure to instantiate Queue
static struct Class_Descriptor _Queue_Class_Descriptor = {
	sizeof(Queue),
	&vtbl
};
const void * Queue_Class_Descriptor = &_Queue_Class_Descriptor;

// Private fields for Queue
	
// Private class method declarations for Queue
// ...

// Private instance method declarations for Queue
// ...

/// Private overrides for 'Object' virtual methods (implementation)

/**
* @brief: Queue constructor.
* @param self: A reference to the current instance of Queue
* @param args: Variadic args list as follows:
* - type: desc
* @return: Object* - The constructed object instance.
*/
static Object* _Object_Ctor(Object * self, va_list args)
{
	Queue* _self = (Queue*)self;
	_self->cap = 200;
	_self->Q = calloc(_self->cap, sizeof(uint16_t));
	_self->front = 0;
	_self->rear = 0;
	_self->els = 0;
	return self;
}

/**
* @brief: Queue destructor.
* @param self: A reference to the current instance of Queue
* @return: Object* - The object to be freed.
*/
static Object* _Object_Dtor(Object * self)
{
	Queue* _self = (Queue*)self;
	if (_self->Q)
		free(_self->Q); _self->Q = 0;
	return self;
}

/**
* @brief: Returns the type of the class.
* @param self: A reference to the current instance of Queue
* @return: const char* const - The string that identifies the class.
*/
static const char* const _Object_Type_Descriptor(Object * self)
{
	return type_string;
}

/**
* @brief: Returns the string representation of the instantiated object.
* @param self: A reference to the current instance of Queue
* @return: const char* const - The string that describes the instantiated object.
*/
static const char* const _Object_Descriptor(Object * self)
{
	return "<Queue>";
}

/**
* @brief: Returns 0 if object is not the same instance as another.
* @param self: A reference to the current instance of Queue.
* @param obj: A reference to the another instance of Queue.
* @return: unsigned int: 0 if not equal.
*/
static unsigned int _Object_Equals(Object* self, Object* obj)
{
	return Object_Equals(self,obj);
}

// Private class methods for Queue
// ...

// Private instance methods for Queue
// ...

// Public class methods for Queue
// ...

// Public instance methods for Queue

/**
* @brief: adds a word to the queue.
* @param self: A reference to the current instance of Queue.
* @param element: a word element.
*/
void Queue_Enqueue(Queue* self, uint16_t element){
	if (self->els == self->cap-1){
		self->Q = (uint16_t*)realloc(self->Q, self->cap*2 * sizeof(uint16_t));
		self->cap = self->cap*2;
	}
	self->Q[self->rear] = element;
	self->rear++;
	self->els++;
}

/**
* @brief: removes a word from the queue.
* @param self: A reference to the current instance of Queue.
* @return: word element which was removed
*/
uint16_t Queue_Dequeue(Queue* self){
	if(Queue_Is_Empty(self)){
		_err("Queue is empty, cannot dequeue.", NULL);
		return 0;
	} else {
		uint16_t rval = *(self->Q + self->front);
		self->front = (self->front+1);
		return rval;
	}
}

/**
* @brief: checks front element of queue.
* @param self: A reference to the current instance of Queue.
* @return: word element at front of queue
*/
uint16_t Queue_Front(Queue* self){
	if(Queue_Is_Empty(self)){
		_err("Queue is empty, cannot get front element.", NULL);
		return 0;
	} else {
		return self->Q[self->front];
	}
	
}

/**
* @brief: checks number of elements in queue.
* @param self: A reference to the current instance of Queue.
* @return int16_t: number of elements in queue.
*/
inline uint16_t Queue_Size(Queue* self){
	return self->rear - self->front;
}

/**
* @brief: checks if queue has no elements.
* @param self: A reference to the current instance of Queue.
* @return int16_t: 0 if false else >0.
*/
inline uint16_t Queue_Is_Empty(Queue* self){
	return self->front == self->rear;
}
