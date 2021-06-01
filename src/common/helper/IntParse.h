/**
 * @file IntParse.h
 * @brief Parse signed integer without minding endianness
**/

#include "m2tp-common/typedef.h"

// big-endian 2 bytes => signed short int
extern signed short int IntParse_16bits(const m2tp_byte input[2]);

// big-endian 4 bytes => signed long int
extern signed long int IntParse_32bits(const m2tp_byte input[4]);

// big-endian 8 bytes => signed long long int
extern signed long long int IntParse_64bits(const m2tp_byte input[8]);
