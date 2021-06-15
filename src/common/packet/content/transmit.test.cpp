/**
 * @file response_register_topic.test.cpp
 * @brief Unit test for response_register_topic.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "transmit.h"
}

TEST(PacketContent_Transmit, Serialize)
{
  packet_content_Transmit input;
  input.source = 0x03;
  input.target = 0x81;

  const char data[] = "{\r\n\"id\":7,\r\n\t\"name\":\"Athaariq A.\",\r\n\t\"logged\":true\r\n}";
  input.data = (m2tp_bytes)data;

  m2tp_byte dataSize = sizeof(data);
  input.dataSize = dataSize;

  m2tp_byte outputSize = 0;
  m2tp_bytes output = packet_content_Transmit_serialize(&input, &outputSize);

  ASSERT_EQ(outputSize, dataSize + 2) << "Invalid output size";

  EXPECT_EQ(output[0], 0x03);
  EXPECT_EQ(output[1], 0x81);
  EXPECT_STREQ((const char *)&output[2], data);

  free(output);
}

TEST(PacketContent_Transmit, Parse)
{
  const char data[] = "{\r\n\"id\":7,\r\n\t\"name\":\"Athaariq A.\",\r\n\t\"logged\":true\r\n}";
  m2tp_byte dataSize = sizeof(data);

  m2tp_byte inputSize = dataSize + 2;
  m2tp_bytes input = (m2tp_bytes)malloc(inputSize);

  input[0] = 0x03;
  input[1] = 0x81;

  for (m2tp_byte i = 0; i < dataSize; i++)
    input[i + 2] = data[i];

  packet_content_Transmit output;
  packet_content_Transmit_parse(input, inputSize, &output);
  free(input);

  EXPECT_EQ(output.source, 0x03);
  EXPECT_EQ(output.target, 0x81);
  ASSERT_EQ(output.dataSize, dataSize) << "Output has invalid data size";
  EXPECT_STREQ((const char *)output.data, data);
  free(output.data);
}
