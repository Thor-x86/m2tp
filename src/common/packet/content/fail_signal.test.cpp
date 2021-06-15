/**
 * @file fail_signal.test.cpp
 * @brief Unit test for fail_signal.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "fail_signal.h"
#include "m2tp-common/errors.h"
}

TEST(PacketContent_FailSignal, Serialize)
{
  packet_content_FailSignal input;
  input.errorCode = M2TP_ERROR_DATA_CORRUPT;

  m2tp_byte outputSize = 0;
  m2tp_bytes output = packet_content_FailSignal_serialize(&input, &outputSize);

  ASSERT_EQ(outputSize, 1);
  EXPECT_EQ(output[0], M2TP_ERROR_DATA_CORRUPT);

  free(output);
}

TEST(PacketContent_FailSignal, Parse)
{
  m2tp_bytes input = (m2tp_bytes)malloc(1);
  input[0] = M2TP_ERROR_DATA_CORRUPT;

  packet_content_FailSignal output;
  packet_content_FailSignal_parse(input, 1, &output);
  free(input);

  EXPECT_EQ(output.errorCode, M2TP_ERROR_DATA_CORRUPT);
}
