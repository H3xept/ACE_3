#include "utilities.h"
#include <stdint.h>

inline int16_t unsigned_to_signed(uint16_t word)
{
	return *(int16_t*) &word;
} 

inline uint16_t signed_to_unsigned(int16_t word)
{
	return *(uint16_t*) &word;
}
