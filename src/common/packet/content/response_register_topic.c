/**
 * @file request_register_topic.c
 * @brief Implementation of request_register_topic.h
**/

#include <stdlib.h>

#include "response_register_topic.h"

void packet_content_ResponseRegisterTopic_serialize(
    const packet_content_ResponseRegisterTopic *input, m2tp_bytes output, m2tp_byte *outputSizePtr)
{
  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = RESPONSE_REGISTER_TOPIC_SIZE;

  // Insert topicID to output
  output[0] = input->ID;
}

void packet_content_ResponseRegisterTopic_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_ResponseRegisterTopic *output)
{
  // Abort if input size is too small
  if (inputSize < RESPONSE_REGISTER_TOPIC_SIZE)
    return;

  // Insert topicID to output
  output->ID = input[0];
}
