#ifndef __ARCH_CONSTS__
#define __ARCH_CONSTS__

#define WORD_SIZE 16
#define MIN_BITS_ADDRESSABLE 16
#define ADDR_LENGTH 12
#define WORDS_IN_MEM (int)(((pow(2,ADDR_LENGTH))*MIN_BITS_ADDRESSABLE)/WORD_SIZE)
#define TOTAL_MEM WORD_SIZE*WORDS_IN_MEM
// Arbitrary: we just assumed that the stack will be big enough being 1/4 of total memory.
#define MAX_PROG_SIZE ((3.0/4.0)*TOTAL_MEM) 
#endif
