/**
 * @file IntSerialize.c
 * @brief Implementation of IntSerialize.h
**/

#include "IntSerialize.h"

void IntSerialize_16bits(signed short int input, m2tp_byte output[2])
{
  output[0] = input & 0xFFU;
  output[1] = (input >> 8) & 0xFFU;
}

void IntSerialize_32bits(signed long int input, m2tp_byte output[4])
{
  IntSerialize_16bits(input, output);
  output[2] = (input >> 16) & 0xFFU;
  output[3] = (input >> 24) & 0xFFU;
}

void IntSerialize_64bits(signed long long int input, m2tp_byte output[8])
{
  IntSerialize_32bits(input, output);
  output[4] = (input >> 32) & 0xFFU;
  output[5] = (input >> 40) & 0xFFU;
  output[6] = (input >> 48) & 0xFFU;
  output[7] = (input >> 56) & 0xFFU;
}
