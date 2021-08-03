/**
 * @file fail_signal.test.cpp
 * @brief Unit test for fail_signal.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "fail_signal.h"
#include "m2tp/errors.h"
}

TEST(PacketContent_FailSignal, Serialize)
{
  packet_content_FailSignal input;
  input.errorCode = M2TP_ERROR_DATA_CORRUPT;

  m2tp_byte outputSize = 0;
  m2tp_byte output[255];
  packet_content_FailSignal_serialize(&input, output, &outputSize);

  ASSERT_EQ(outputSize, 1);
  EXPECT_EQ(output[0], M2TP_ERROR_DATA_CORRUPT);
}

TEST(PacketContent_FailSignal, Parse)
{
  m2tp_byte input[FAIL_SIGNAL_SIZE] = {M2TP_ERROR_DATA_CORRUPT};

  packet_content_FailSignal output;
  packet_content_FailSignal_parse(input, 1, &output);

  EXPECT_EQ(output.errorCode, M2TP_ERROR_DATA_CORRUPT);
}
