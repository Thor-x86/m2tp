/**
 * @file Packet.c
 * @brief Implements Packet.h functionality
**/

#include <stdlib.h>

#include "Packet.h"

m2tp_bytes Packet_serialize(const Packet *input, m2tp_byte *outputSizePtr)
{
  // Resolve output size
  m2tp_byte outputSize = input->contentSize + 2;

  // Send output size via pointer, if not NULL
  if (outputSizePtr != NULL)
    *outputSizePtr = outputSize;

  // Allocate memory for output
  m2tp_bytes output = (m2tp_bytes)malloc(outputSize);

  // Insert command
  output[0] = input->command;

  // Insert contentSize
  output[1] = input->contentSize;

  // Insert raw content
  for (m2tp_byte i = 0; i < input->contentSize; i++)
    output[i + 2] = input->content[i];

  return output;
}

void Packet_parse(const m2tp_bytes input, Packet *output)
{
  // FIXME: Technique to check valid .contentSize here

  // Insert command
  output->command = input[0];

  // Insert contentSize
  output->contentSize = input[1];

  // Allocate RAM for content
  output->content = (m2tp_bytes)malloc(output->contentSize);

  // Insert content
  for (m2tp_byte i = 0; i < output->contentSize; i++)
    output->content[i] = input[i + 2];
}
