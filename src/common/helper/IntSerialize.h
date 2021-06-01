/**
 * @file IntSerialize.h
 * @brief Serialize signed integer without minding endianness
**/

#include "m2tp-common/typedef.h"

// signed short int => big-endian 2 bytes
extern void IntSerialize_16bits(signed short int input, m2tp_byte output[2]);

// signed long int => big-endian 4 bytes
extern void IntSerialize_32bits(signed long int input, m2tp_byte output[4]);

// signed long long int => big-endian 8 bytes
extern void IntSerialize_64bits(signed long long int input, m2tp_byte output[8]);
