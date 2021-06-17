/**
 * @file announcement_quit.test.cpp
 * @brief Unit test for announcement_quit.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "announcement_quit.h"
}

TEST(PacketContent_AnnouncementQuit, Serialize)
{
  packet_content_AnnouncementQuit input;
  input.address = 0x7f;

  m2tp_byte outputSize = 0;
  m2tp_byte output[ANNOUNCEMENT_QUIT_SIZE];
  packet_content_AnnouncementQuit_serialize(&input, output, &outputSize);

  ASSERT_EQ(outputSize, ANNOUNCEMENT_QUIT_SIZE);
  EXPECT_EQ(output[0], 0x7F);
}

TEST(PacketContent_AnnouncementQuit, Parse)
{
  m2tp_byte input[1] = {0x7F};

  packet_content_AnnouncementQuit output;
  packet_content_AnnouncementQuit_parse(input, 1, &output);

  EXPECT_EQ(output.address, 0x7F);
}
