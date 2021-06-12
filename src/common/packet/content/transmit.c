/**
 * @file transmit.c
 * @brief Implementation of transmit.h
**/

#include <stdlib.h>

#include "transmit.h"

m2tp_bytes packet_content_Transmit_serialize(
    const packet_content_Transmit *input, m2tp_byte *outputSizePtr)
{
  // Resolve output size
  m2tp_byte outputSize = 2 + input->dataSize;

  // Allocate memory with fixed size
  m2tp_bytes output = (m2tp_bytes)malloc(outputSize);

  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = outputSize;

  // Insert source and target channel
  output[0] = input->source;
  output[1] = input->target;

  // Insert the data
  for (m2tp_byte i = 0; i < input->dataSize; i++)
    output[i + 2] = input->data[i];

  return output;
}

void packet_content_Transmit_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_Transmit *output)
{
  // Abort if input size is too small
  if (inputSize < 2)
    return;

  // Insert source and target channel
  output->source = input[0];
  output->target = input[1];

  // Resolve data size, then allocate
  output->dataSize = inputSize - 2;
  output->data = (m2tp_bytes)malloc(output->dataSize);

  // Insert data to output
  for (m2tp_byte i = 0; i < output->dataSize; i++)
    output->data[i] = input[i + 2];
}
