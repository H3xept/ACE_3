#include <stdio.h>
#include <stdlib.h>
#include "./oop/umbrella.h"
#include "./oop/protocols/MemoryDelegate.h"

int main(int argc, char const *argv[])
{
	//testing  queue
	Queue* aq = alloc_init(Queue_Class_Descriptor);
	Queue* bq = alloc_init(Queue_Class_Descriptor);
	for(int i = 0;i<10;i++){
	Queue_Enqueue(aq,i);
	Queue_Enqueue(bq,i*5);
	}
	while(!Queue_Is_Empty(aq)){
		printf("%d	%d\n",Queue_Dequeue(aq),Queue_Dequeue(bq));
	}
	
	//testing alu
	ALU* alu = alloc_init(ALU_Class_Descriptor);
	word_t w = ALU_Multiply(alu,5000,10);
	printf("%d",w);
	
	
	/*
	MemoryController* mc = alloc_init(MemoryController_Class_Descriptor);
	struct MemoryDelegate* delegate = mc->memoryDelegateVptr;
	delegate->MemoryDelegate_Clear_Memory(delegate);
	*/
	/*
	IO* io = alloc_init(IO_Class_Descriptor);
	struct IODelegate* ioDelegate = io->iODelegateVptr;
	ioDelegate->IODelegate_Get_Word_From_Input_Queue(ioDelegate);
	*/
	
}
