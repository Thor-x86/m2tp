/**
 * @file Packet.test.cpp
 * @brief Unit test for Packet.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "Flag.h"
}

TEST(Flag, Set)
{
  m2tp_byte flags = 0b01101001;
  Flag_set(flags, 4);
  EXPECT_EQ(flags, 0b01111001);
}

TEST(Flag, Unset)
{
  m2tp_byte flags = 0b01101001;
  Flag_unset(flags, 5);
  EXPECT_EQ(flags, 0b01001001);
}

TEST(Flag, Check)
{
  m2tp_byte flags = 0b01101001;
  EXPECT_TRUE(Flag_check(flags, 5));
  EXPECT_FALSE(Flag_check(flags, 4));
}
