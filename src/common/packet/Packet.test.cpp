/**
 * @file Packet.test.cpp
 * @brief Unit test for Packet.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "Packet.h"
#include "m2tp/commands.h"
}

TEST(Packet, Serialize)
{
  Packet input;
  input.command = M2TP_COMMAND_ANNOUNCEMENT_JOIN;
  input.contentSize = 6;

  m2tp_byte inputContent[input.contentSize];
  inputContent[0] = 0x7F;
  inputContent[1] = 'L';
  inputContent[2] = 'o';
  inputContent[3] = 'r';
  inputContent[4] = 'e';
  inputContent[5] = 'm';
  input.content = inputContent;

  m2tp_byte outputSize = 0;
  m2tp_byte output[255];
  Packet_serialize(&input, output, &outputSize);

  ASSERT_EQ(outputSize, 8) << "Invalid output size";

  EXPECT_EQ(output[0], M2TP_COMMAND_ANNOUNCEMENT_JOIN);
  EXPECT_EQ(output[1], 6);
  EXPECT_EQ(output[2], 0x7F);
  EXPECT_EQ(output[3], 'L');
  EXPECT_EQ(output[4], 'o');
  EXPECT_EQ(output[5], 'r');
  EXPECT_EQ(output[6], 'e');
  EXPECT_EQ(output[7], 'm');
}

TEST(Packet, Parse)
{
  m2tp_byte input[8];
  input[0] = M2TP_COMMAND_ANNOUNCEMENT_JOIN;
  input[1] = 6;
  input[2] = 0x7F;
  input[3] = 'L';
  input[4] = 'o';
  input[5] = 'r';
  input[6] = 'e';
  input[7] = 'm';

  m2tp_byte outputContent[255];
  Packet output;
  output.content = outputContent;
  Packet_parse(input, &output);

  EXPECT_EQ(output.command, M2TP_COMMAND_ANNOUNCEMENT_JOIN);
  ASSERT_EQ(output.contentSize, 6) << "Invalid content size";
  EXPECT_EQ(output.content[0], 0x7f);
  EXPECT_EQ(output.content[1], 'L');
  EXPECT_EQ(output.content[2], 'o');
  EXPECT_EQ(output.content[3], 'r');
  EXPECT_EQ(output.content[4], 'e');
  EXPECT_EQ(output.content[5], 'm');
}
