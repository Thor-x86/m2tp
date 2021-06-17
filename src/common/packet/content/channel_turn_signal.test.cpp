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
  m2tp_byte output[255];
  packet_content_ChannelTurnSignal_serialize(&input, output, &outputSize);

  ASSERT_EQ(outputSize, CHANNEL_TURN_SIGNAL_SIZE);
  EXPECT_EQ(output[0], 0x7F);
}

TEST(PacketContent_ChannelTurnSignal, Parse)
{
  m2tp_byte input[CHANNEL_TURN_SIGNAL_SIZE] = {0x7F};

  packet_content_ChannelTurnSignal output;
  packet_content_ChannelTurnSignal_parse(input, 1, &output);

  EXPECT_EQ(output.channel, 0x7F);
}
