/**
 * @file request_register_topic.c
 * @brief Implementation of request_register_topic.h
**/

#include <stdlib.h>

#include "request_register_topic.h"

m2tp_bytes packet_content_RequestRegisterTopic_serialize(
    const packet_content_RequestRegisterTopic *input, unsigned short *outputSizePtr)
{
  // Resolve topic name size
  m2tp_byte nameSize = 0;
  for (nameSize; nameSize < 255; nameSize++)
  {
    if (input->name[nameSize] == '\0')
      break;
  }

  // Allocate memory with fixed size
  m2tp_bytes output = (m2tp_bytes)malloc(nameSize);

  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = nameSize;

  // Insert topic name to output
  for (m2tp_byte i = 0; i < nameSize; i++)
    output[i] = input->name[i];

  return output;
}

void packet_content_RequestRegisterTopic_parse(
    const m2tp_bytes input, unsigned short inputSize, packet_content_RequestRegisterTopic *output)
{
  // Allocate for topic name string
  // NOTE: output size equals name size
  output->name = (char *)malloc(inputSize + 1);

  // Insert topic name
  for (m2tp_byte i = 0; i < inputSize; i++)
    output->name[i] = input[i];

  // Insert NULL character to topic name to prevent segfault error
  output->name[inputSize] = '\0';
}
