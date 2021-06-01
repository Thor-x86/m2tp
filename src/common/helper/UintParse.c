/**
 * @file UintParse.c
 * @brief Implementation of UintParse.h
**/

#include "UintParse.h"

unsigned short int UintParse_16bits(const m2tp_byte input[2])
{
  unsigned short int output;
  output |= input[0] & 0xFFU;
  output |= (input[1] & 0xFFU) << 8;
  return output;
}

unsigned long int UintParse_32bits(const m2tp_byte input[4])
{
  unsigned long int output = UintParse_16bits(input);
  output |= (input[2] & 0xFFUL) << 16;
  output |= (input[3] & 0xFFUL) << 24;
  return output;
}

unsigned long long int UintParse_64bits(const m2tp_byte input[8])
{
  unsigned long long int output = UintParse_32bits(input);
  output |= (input[4] & 0xFFULL) << 32;
  output |= (input[5] & 0xFFULL) << 40;
  output |= (input[6] & 0xFFULL) << 48;
  output |= (input[7] & 0xFFULL) << 56;
  return output;
}
