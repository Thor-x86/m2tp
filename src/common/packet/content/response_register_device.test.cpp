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
  packet_content_ResponseRegisterDevice input;
  input.address = 0x7F;

  m2tp_byte outputSize = 0;
  m2tp_byte output[255];
  packet_content_ResponseRegisterDevice_serialize(&input, output, &outputSize);

  ASSERT_EQ(outputSize, RESPONSE_REGISTER_DEVICE_SIZE) << "Invalid output size";

  EXPECT_EQ(output[0], 0x7F);
}

TEST(PacketContent_ResponseRegisterDevice, Parse)
{
  m2tp_byte input[RESPONSE_REGISTER_DEVICE_SIZE] = {0x7F};

  packet_content_ResponseRegisterDevice output;
  packet_content_ResponseRegisterDevice_parse(input, 2, &output);

  EXPECT_EQ(output.address, 0x7F);
}
