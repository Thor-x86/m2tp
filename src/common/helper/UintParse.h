/**
 * @file UintParse.h
 * @brief Parse unsigned integer without minding endianness
**/

#include "m2tp-common/typedef.h"

// big-endian 2 bytes => unsigned short int
extern unsigned short int UintParse_16bits(const m2tp_byte input[2]);

// big-endian 4 bytes => unsigned long int
extern unsigned long int UintParse_32bits(const m2tp_byte input[4]);

// big-endian 8 bytes => unsigned long long int
extern unsigned long long int UintParse_64bits(const m2tp_byte input[8]);
