/**
* Filename: utilities.c
* Authors: Leonardo Cascianelli,Rory Brown,Ewan Skene
* Date: 2018-11-16
* 
* Description: utility functions
*/

#include "utilities.h"
#include <stdint.h>

/**
* @brief: converts unsigned int16_t to signed.
* @param word: uint16_t to be converted
* @return: resulting signed number
*/
inline int16_t unsigned_to_signed(uint16_t word)
{
	return *(int16_t*) &word;
} 

/**
* @brief: converts signed int16_t to unsigned.
* @param word: int16_t to be converted
* @return: resulting unsigned number
*/
inline uint16_t signed_to_unsigned(int16_t word)
{
	return *(uint16_t*) &word;
}
