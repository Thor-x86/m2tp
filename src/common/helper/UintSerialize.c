/**
 * @file UintSerialize.c
 * @brief Implementation of UintSerialize.h
**/

#include "UintSerialize.h"

void UintSerialize_16bits(unsigned short int input, m2tp_byte output[2])
{
  output[0] = input & 0xFFU;
  output[1] = (input >> 8) & 0xFFU;
}

void UintSerialize_32bits(unsigned long int input, m2tp_byte output[4])
{
  UintSerialize_16bits(input, output);
  output[2] = (input >> 16) & 0xFFU;
  output[3] = (input >> 24) & 0xFFU;
}

void UintSerialize_64bits(unsigned long long int input, m2tp_byte output[8])
{
  UintSerialize_32bits(input, output);
  output[4] = (input >> 32) & 0xFFU;
  output[5] = (input >> 40) & 0xFFU;
  output[6] = (input >> 48) & 0xFFU;
  output[7] = (input >> 56) & 0xFFU;
}
