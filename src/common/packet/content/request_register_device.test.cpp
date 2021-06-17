/**
 * @file request_register_device.test.cpp
 * @brief Unit test for request_register_device.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "request_register_device.h"
}

TEST(PacketContent_RequestRegisterDevice, Serialize)
{
  m2tp_byte dice = 255 * (rand() / RAND_MAX);

  packet_content_RequestRegisterDevice input;
  input.dice = dice;
  input.deviceClass = "Lorem Ipsum";

  m2tp_byte outputSize = 0;
  m2tp_byte output[255];
  packet_content_RequestRegisterDevice_serialize(&input, output, &outputSize);

  ASSERT_EQ(outputSize, 12) << "Invalid output size";

  EXPECT_EQ(output[0], dice);
  EXPECT_EQ(output[1], 'L');
  EXPECT_EQ(output[2], 'o');
  EXPECT_EQ(output[3], 'r');
  EXPECT_EQ(output[4], 'e');
  EXPECT_EQ(output[5], 'm');
  EXPECT_EQ(output[6], ' ');
  EXPECT_EQ(output[7], 'I');
  EXPECT_EQ(output[8], 'p');
  EXPECT_EQ(output[9], 's');
  EXPECT_EQ(output[10], 'u');
  EXPECT_EQ(output[11], 'm');
}

TEST(PacketContent_RequestRegisterDevice, Parse)
{
  m2tp_byte dice = 255 * (rand() / RAND_MAX);

  m2tp_byte input[255];
  input[0] = dice;
  input[1] = 'L';
  input[2] = 'o';
  input[3] = 'r';
  input[4] = 'e';
  input[5] = 'm';
  input[6] = ' ';
  input[7] = 'I';
  input[8] = 'p';
  input[9] = 's';
  input[10] = 'u';
  input[11] = 'm';

  char deviceClass[254];
  packet_content_RequestRegisterDevice output;
  output.deviceClass = deviceClass;
  packet_content_RequestRegisterDevice_parse(input, 12, &output);

  EXPECT_EQ(output.dice, dice);
  EXPECT_STREQ(output.deviceClass, "Lorem Ipsum");
}
