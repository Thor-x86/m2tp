/**
 * @file IntParse.c
 * @brief Implementation of IntParse.h
**/

#include "IntParse.h"

signed short int IntParse_16bits(const m2tp_byte input[2])
{
  signed short int output;
  output |= input[0] & 0xFF;
  output |= (input[1] & 0xFF) << 8;
  return output;
}

signed long int IntParse_32bits(const m2tp_byte input[4])
{
  signed long int output = IntParse_16bits(input);
  output |= (input[2] & 0xFFL) << 16;
  output |= (input[3] & 0xFFL) << 24;
  return output;
}

signed long long int IntParse_64bits(const m2tp_byte input[8])
{
  signed long long int output = IntParse_32bits(input);
  output |= (input[4] & 0xFFLL) << 32;
  output |= (input[5] & 0xFFLL) << 40;
  output |= (input[6] & 0xFFLL) << 48;
  output |= (input[7] & 0xFFLL) << 56;
  return output;
}
