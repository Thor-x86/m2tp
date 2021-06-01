/**
 * @file UintSerialize.h
 * @brief Serialize unsigned integer without minding endianness
**/

#include "m2tp-common/typedef.h"

// unsigned short int => big-endian 2 bytes
extern void UintSerialize_16bits(unsigned short int input, m2tp_byte output[2]);

// unsigned long int => big-endian 4 bytes
extern void UintSerialize_32bits(unsigned long int input, m2tp_byte output[4]);

// unsigned long long int => big-endian 8 bytes
extern void UintSerialize_64bits(unsigned long long int input, m2tp_byte output[8]);
