/**
 * @file IntParse.test.cpp
 * @brief Test unit for IntParse.h
**/

#include "IntParse.h"
#include "IntParse.c"

#include "gtest/gtest.h"

TEST(IntParse, With16Bits)
{
  m2tp_bytes input = (m2tp_bytes)malloc(sizeof(m2tp_byte) * 2);
  input[1] = 0x32;
  input[0] = 0x10;

  short output = IntParse_16bits(input);
  free(input);

  EXPECT_EQ(output, 0x3210);
}

TEST(IntParse, With32Bits)
{
  m2tp_bytes input = (m2tp_bytes)malloc(sizeof(m2tp_byte) * 4);
  input[3] = 0x76;
  input[2] = 0x54;
  input[1] = 0x32;
  input[0] = 0x10;

  long output = IntParse_32bits(input);
  free(input);

  EXPECT_EQ(output, 0x76543210L);
}

TEST(IntParse, With64Bits)
{
  m2tp_bytes input = (m2tp_bytes)malloc(sizeof(m2tp_byte) * 8);
  input[7] = 0xFE;
  input[6] = 0xDC;
  input[5] = 0xBA;
  input[4] = 0x98;
  input[3] = 0x76;
  input[2] = 0x54;
  input[1] = 0x32;
  input[0] = 0x10;

  long long output = IntParse_64bits(input);
  free(input);

  EXPECT_EQ(output, 0xFEDCBA9876543210LL);
}
