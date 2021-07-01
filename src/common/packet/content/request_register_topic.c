/**
 * @file request_register_topic.c
 * @brief Implementation of request_register_topic.h
**/

#include <stdlib.h>

#include "request_register_topic.h"

void packet_content_RequestRegisterTopic_serialize(
    const char *topicName, m2tp_bytes output, m2tp_byte *outputSizePtr)
{
  // Resolve topic name size
  m2tp_byte topicNameSize = 0;
  for (topicNameSize; topicNameSize < 255; topicNameSize++)
  {
    if (topicName[topicNameSize] == '\0')
      break;
  }

  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = topicNameSize;

  // Insert topic name to output
  for (m2tp_byte i = 0; i < topicNameSize; i++)
    output[i] = topicName[i];
}

void packet_content_RequestRegisterTopic_parse(
    const m2tp_bytes input, m2tp_byte inputSize, char *topicName)
{
  // Insert topic name
  for (m2tp_byte i = 0; i < inputSize; i++)
    topicName[i] = input[i];

  // Insert NULL character to topic name to prevent segfault error
  topicName[inputSize] = '\0';
}
