/**
 * @file response_register_topic.test.cpp
 * @brief Unit test for response_register_topic.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "response_register_topic.h"
}

TEST(PacketContent_ResponseRegisterTopic, Serialize)
{
  packet_content_ResponseRegisterTopic input;
  input.ID = 0xF2;

  m2tp_byte outputSize = 0;
  m2tp_byte output[RESPONSE_REGISTER_TOPIC_SIZE];
  packet_content_ResponseRegisterTopic_serialize(&input, output, &outputSize);

  ASSERT_EQ(outputSize, RESPONSE_REGISTER_TOPIC_SIZE) << "Invalid output size";
  EXPECT_EQ(output[0], 0xF2);
}

TEST(PacketContent_ResponseRegisterTopic, Parse)
{
  m2tp_byte input[1] = {0xF2};

  packet_content_ResponseRegisterTopic output;
  packet_content_ResponseRegisterTopic_parse(input, 1, &output);

  EXPECT_EQ(output.ID, 0xF2);
}
