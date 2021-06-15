/**
 * @file channel_turn_signal.test.cpp
 * @brief Unit test for channel_turn_signal.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "channel_turn_signal.h"
}

TEST(PacketContent_ChannelTurnSignal, Serialize)
{
  packet_content_ChannelTurnSignal input;
  input.channel = 0x7F;

  m2tp_byte outputSize = 0;
  m2tp_bytes output = packet_content_ChannelTurnSignal_serialize(&input, &outputSize);

  ASSERT_EQ(outputSize, 1);
  EXPECT_EQ(output[0], 0x7F);

  free(output);
}

TEST(PacketContent_ChannelTurnSignal, Parse)
{
  m2tp_bytes input = (m2tp_bytes)malloc(1);
  input[0] = 0x7F;

  packet_content_ChannelTurnSignal output;
  packet_content_ChannelTurnSignal_parse(input, 1, &output);
  free(input);

  EXPECT_EQ(output.channel, 0x7F);
}
