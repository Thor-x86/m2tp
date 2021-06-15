/**
 * @file response_register_device.test.cpp
 * @brief Unit test for response_register_device.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "response_register_device.h"
}

TEST(PacketContent_ResponseRegisterDevice, Serialize)
{
  m2tp_byte dice = 255 * (rand() / RAND_MAX);

  packet_content_ResponseRegisterDevice input;
  input.dice = dice;
  input.address = 0x7F;

  m2tp_byte outputSize = 0;
  m2tp_bytes output = packet_content_ResponseRegisterDevice_serialize(&input, &outputSize);

  ASSERT_EQ(outputSize, 2) << "Invalid output size";

  EXPECT_EQ(output[0], dice);
  EXPECT_EQ(output[1], 0x7F);

  free(output);
}

TEST(PacketContent_ResponseRegisterDevice, Parse)
{
  m2tp_byte dice = 255 * (rand() / RAND_MAX);

  m2tp_bytes input = (m2tp_bytes)malloc(2);
  input[0] = dice;
  input[1] = 0x7F;

  packet_content_ResponseRegisterDevice output;
  packet_content_ResponseRegisterDevice_parse(input, 2, &output);
  free(input);

  EXPECT_EQ(output.dice, dice);
  EXPECT_EQ(output.address, 0x7F);
}
