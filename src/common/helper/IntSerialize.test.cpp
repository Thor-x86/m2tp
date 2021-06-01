/**
 * @file IntSerialize.test.cpp
 * @brief Test unit for IntSerialize.h
**/

#include "IntSerialize.h"
#include "IntSerialize.c"

#include "gtest/gtest.h"

TEST(IntSerialize, With16Bits)
{
  short input = 0x3210;
  m2tp_bytes output = (m2tp_bytes)malloc(sizeof(m2tp_byte) * 2);
  IntSerialize_16bits(input, output);

  EXPECT_EQ(output[0], 0x10);
  EXPECT_EQ(output[1], 0x32);

  free(output);
}

TEST(IntSerialize, With32Bits)
{
  long input = 0x76543210L;
  m2tp_bytes output = (m2tp_bytes)malloc(sizeof(m2tp_byte) * 4);
  IntSerialize_32bits(input, output);

  EXPECT_EQ(output[0], 0x10);
  EXPECT_EQ(output[1], 0x32);
  EXPECT_EQ(output[2], 0x54);
  EXPECT_EQ(output[3], 0x76);

  free(output);
}

TEST(IntSerialize, With64Bits)
{
  long long input = 0xFEDCBA9876543210LL;
  m2tp_bytes output = (m2tp_bytes)malloc(sizeof(m2tp_byte) * 8);
  IntSerialize_64bits(input, output);

  EXPECT_EQ(output[0], 0x10);
  EXPECT_EQ(output[1], 0x32);
  EXPECT_EQ(output[2], 0x54);
  EXPECT_EQ(output[3], 0x76);
  EXPECT_EQ(output[4], 0x98);
  EXPECT_EQ(output[5], 0xBA);
  EXPECT_EQ(output[6], 0xDC);
  EXPECT_EQ(output[7], 0xFE);

  free(output);
}
