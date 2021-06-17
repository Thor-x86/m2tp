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
  m2tp_byte output[255];
  packet_content_ResponseRegisterDevice_serialize(&input, output, &outputSize);

  ASSERT_EQ(outputSize, RESPONSE_REGISTER_DEVICE_SIZE) << "Invalid output size";

  EXPECT_EQ(output[0], dice);
  EXPECT_EQ(output[1], 0x7F);
}

TEST(PacketContent_ResponseRegisterDevice, Parse)
{
  m2tp_byte dice = 255 * (rand() / RAND_MAX);

  m2tp_byte input[RESPONSE_REGISTER_DEVICE_SIZE] = {dice, 0x7F};

  packet_content_ResponseRegisterDevice output;
  packet_content_ResponseRegisterDevice_parse(input, 2, &output);

  EXPECT_EQ(output.dice, dice);
  EXPECT_EQ(output.address, 0x7F);
}
