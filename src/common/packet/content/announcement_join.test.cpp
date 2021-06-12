/**
 * @file announcement_join.test.cpp
 * @brief Unit test for announcement_join.h
**/

#include "announcement_join.h"
#include "announcement_join.c"

#include "gtest/gtest.h"

TEST(PacketContent_AnnouncementJoin, Serialize)
{
  packet_content_AnnouncementJoin input;
  input.address = 0x7F;
  input.deviceClass = "Lorem Ipsum";

  m2tp_byte outputSize = 0;
  m2tp_bytes output = packet_content_AnnouncementJoin_serialize(&input, &outputSize);

  ASSERT_EQ(outputSize, 12) << "Invalid output size";

  EXPECT_EQ(output[0], 0x7f);
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

  free(output);
}

TEST(PacketContent_AnnouncementJoin, Parse)
{
  m2tp_bytes input = (m2tp_bytes)malloc(12);
  input[0] = 0x7f;
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

  packet_content_AnnouncementJoin output;
  packet_content_AnnouncementJoin_parse(input, 12, &output);
  free(input);

  EXPECT_EQ(output.address, 0x7f);
  EXPECT_STREQ(output.deviceClass, "Lorem Ipsum");
  free(output.deviceClass);
}
