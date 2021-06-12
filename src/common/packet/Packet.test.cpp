/**
 * @file Packet.test.cpp
 * @brief Unit test for Packet.h
**/

#include "Packet.h"
#include "Packet.c"

#include "gtest/gtest.h"

#include "m2tp-common/commands.h"

TEST(Packet, Serialize)
{
  Packet input;
  input.command = M2TP_COMMAND_ANNOUNCEMENT_JOIN;
  input.contentSize = 6;

  input.content = (m2tp_bytes)malloc(input.contentSize);
  input.content[0] = 0x7F;
  input.content[1] = 'L';
  input.content[2] = 'o';
  input.content[3] = 'r';
  input.content[4] = 'e';
  input.content[5] = 'm';

  unsigned short outputSize = 0;
  m2tp_bytes output = Packet_serialize(&input, &outputSize);
  free(input.content);

  ASSERT_EQ(outputSize, 9) << "Invalid output size";

  EXPECT_EQ(output[0], M2TP_COMMAND_ANNOUNCEMENT_JOIN);
  EXPECT_EQ(output[1], 0x0U);
  EXPECT_EQ(output[2], 6);
  EXPECT_EQ(output[3], 0x7F);
  EXPECT_EQ(output[4], 'L');
  EXPECT_EQ(output[5], 'o');
  EXPECT_EQ(output[6], 'r');
  EXPECT_EQ(output[7], 'e');
  EXPECT_EQ(output[8], 'm');

  free(output);
}

TEST(Packet, Parse)
{
  m2tp_bytes input = (m2tp_bytes)malloc(9);
  input[0] = M2TP_COMMAND_ANNOUNCEMENT_JOIN;
  input[1] = 0x0U;
  input[2] = 6;
  input[3] = 0x7F;
  input[4] = 'L';
  input[5] = 'o';
  input[6] = 'r';
  input[7] = 'e';
  input[8] = 'm';

  Packet output;
  Packet_parse(input, &output);
  free(input);

  EXPECT_EQ(output.command, M2TP_COMMAND_ANNOUNCEMENT_JOIN);
  ASSERT_EQ(output.contentSize, 6) << "Invalid content size";
  EXPECT_EQ(output.content[0], 0x7f);
  EXPECT_EQ(output.content[1], 'L');
  EXPECT_EQ(output.content[2], 'o');
  EXPECT_EQ(output.content[3], 'r');
  EXPECT_EQ(output.content[4], 'e');
  EXPECT_EQ(output.content[5], 'm');
  free(output.content);
}
