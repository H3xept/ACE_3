#ifndef __MemoryDelegate__
#define __MemoryDelegate__

#include "../constants/var_word_size.h"

struct MemoryDelegate {
	word_t (*MemoryDelegate_Word_At_Address)(struct MemoryDelegate * self, word_t addr) ;
	void (*MemoryDelegate_Set_Word_At_Address)(struct MemoryDelegate * self, word_t addr, word_t word);
	void (*MemoryDelegate_Clear_Memory)(struct MemoryDelegate * self);
	void (*MemoryDelegate_Load_Memory_From_Ptr)(struct MemoryDelegate * self, void* ptr, size_t size);
};

#endif
