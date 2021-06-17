/**
 * @file request_register_topic.c
 * @brief Implementation of request_register_topic.h
**/

#include <stdlib.h>

#include "response_register_topic.h"

void packet_content_ResponseRegisterTopic_serialize(
    const packet_content_ResponseRegisterTopic *input, m2tp_bytes output, m2tp_byte *outputSizePtr)
{
  // Resolve topic name size
  m2tp_byte nameSize = 0;
  for (nameSize; nameSize < 255; nameSize++)
  {
    if (input->name[nameSize] == '\0')
      break;
  }

  // Resolve output size
  m2tp_byte outputSize = 1 + nameSize;

  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = outputSize;

  // Insert topicID to output
  output[0] = input->ID;

  // Insert topic name to output
  for (m2tp_byte i = 0; i < nameSize; i++)
    output[i + 1] = input->name[i];
}

void packet_content_ResponseRegisterTopic_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_ResponseRegisterTopic *output)
{
  // Abort if input size is too small
  if (inputSize < 1)
    return;

  // Insert topicID to output
  output->ID = input[0];

  // Resolve topic name size
  m2tp_byte nameSize = inputSize - 1;

  // Allocate if not allocated yet
  if (output->name == NULL)
    output->name = (char *)malloc(nameSize + 1);

  // Insert topic name to output
  for (m2tp_byte i = 0; i < nameSize; i++)
    output->name[i] = input[i + 1];

  // Insert NULL character to topic name to prevent segfault error
  output->name[nameSize] = '\0';
}
