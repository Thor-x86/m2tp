/**
 * @file announcement_quit.test.cpp
 * @brief Unit test for announcement_quit.h
**/

#include "announcement_quit.h"
#include "announcement_quit.c"

#include "gtest/gtest.h"

TEST(PacketContent_AnnouncementQuit, Serialize)
{
  packet_content_AnnouncementQuit input;
  input.address = 0x7f;

  m2tp_byte outputSize = 0;
  m2tp_bytes output = packet_content_AnnouncementQuit_serialize(&input, &outputSize);

  ASSERT_EQ(outputSize, 1);
  EXPECT_EQ(output[0], 0x7F);

  free(output);
}

TEST(PacketContent_AnnouncementQuit, Parse)
{
  m2tp_bytes input = (m2tp_bytes)malloc(1);
  input[0] = 0x7F;

  packet_content_AnnouncementQuit output;
  packet_content_AnnouncementQuit_parse(input, 1, &output);
  free(input);

  EXPECT_EQ(output.address, 0x7F);
}
