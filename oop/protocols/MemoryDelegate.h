#ifndef __MemoryDelegate__
#define __MemoryDelegate__

#warning Temporary
#define WORD_SIZE 16
#if WORD_SIZE == 16
typedef int16_t word_t;
#else
typedef int32_t word_t;
#endif

struct MemoryDelegate {
	word_t (*MemoryDelegate_Word_At_Address)(struct MemoryDelegate * self, word_t addr) ;
	void (*MemoryDelegate_Set_Word_At_Address)(struct MemoryDelegate * self, word_t addr, word_t word);
	void (*MemoryDelegate_Clear_Memory)(struct MemoryDelegate * self);
	void (*MemoryDelegate_Load_Memory_From_Ptr)(struct MemoryDelegate * self, void* ptr, size_t size);
};

#endif
