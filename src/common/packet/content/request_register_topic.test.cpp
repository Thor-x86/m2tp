/**
 * @file request_register_topic.test.cpp
 * @brief Unit test for request_register_topic.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "request_register_topic.h"
}

TEST(PacketContent_RequestRegisterTopic, Serialize)
{
  packet_content_RequestRegisterTopic input;
  input.name = "Lorem Ipsum";

  m2tp_byte outputSize = 0;
  m2tp_byte output[255];
  packet_content_RequestRegisterTopic_serialize(&input, output, &outputSize);

  ASSERT_EQ(outputSize, 11) << "Invalid output size";

  EXPECT_EQ(output[0], 'L');
  EXPECT_EQ(output[1], 'o');
  EXPECT_EQ(output[2], 'r');
  EXPECT_EQ(output[3], 'e');
  EXPECT_EQ(output[4], 'm');
  EXPECT_EQ(output[5], ' ');
  EXPECT_EQ(output[6], 'I');
  EXPECT_EQ(output[7], 'p');
  EXPECT_EQ(output[8], 's');
  EXPECT_EQ(output[9], 'u');
  EXPECT_EQ(output[10], 'm');
}

TEST(PacketContent_RequestRegisterTopic, Parse)
{
  m2tp_byte input[255];
  input[0] = 'L';
  input[1] = 'o';
  input[2] = 'r';
  input[3] = 'e';
  input[4] = 'm';
  input[5] = ' ';
  input[6] = 'I';
  input[7] = 'p';
  input[8] = 's';
  input[9] = 'u';
  input[10] = 'm';

  char name[254];
  packet_content_RequestRegisterTopic output;
  output.name = name;
  packet_content_RequestRegisterTopic_parse(input, 11, &output);

  EXPECT_STREQ(output.name, "Lorem Ipsum");
}
