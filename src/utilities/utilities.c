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
