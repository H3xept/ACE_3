#include "utilities.h"
#include "../oop/constants/var_word_size.h"

inline word_t signed_to_unsigned(uword_t word){
	return *(word_t*) &word;
} 

inline uword_t unsigned_to_signed(word_t word){
	return *(uword_t*) &word;
}
