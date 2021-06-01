/**
 * @file UintSerialize.test.cpp
 * @brief Test unit for UintSerialize.h
**/

#include "UintSerialize.h"
#include "UintSerialize.c"

#include "gtest/gtest.h"

TEST(UintSerialize, With16Bits)
{
  unsigned short input = 0x3210U;
  m2tp_bytes output = (m2tp_bytes)malloc(sizeof(m2tp_byte) * 2);
  UintSerialize_16bits(input, output);

  EXPECT_EQ(output[0], 0x10);
  EXPECT_EQ(output[1], 0x32);

  free(output);
}

TEST(UintSerialize, With32Bits)
{
  unsigned long input = 0x76543210UL;
  m2tp_bytes output = (m2tp_bytes)malloc(sizeof(m2tp_byte) * 4);
  UintSerialize_32bits(input, output);

  EXPECT_EQ(output[0], 0x10);
  EXPECT_EQ(output[1], 0x32);
  EXPECT_EQ(output[2], 0x54);
  EXPECT_EQ(output[3], 0x76);

  free(output);
}

TEST(UintSerialize, With64Bits)
{
  unsigned long long input = 0xFEDCBA9876543210ULL;
  m2tp_bytes output = (m2tp_bytes)malloc(sizeof(m2tp_byte) * 8);
  UintSerialize_64bits(input, output);

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
