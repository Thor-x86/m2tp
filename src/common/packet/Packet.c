/**
 * @file Packet.c
 * @brief Implements Packet.h functionality
**/

#include <stdlib.h>

#include "Packet.h"

m2tp_bytes Packet_serialize(const Packet *input, unsigned short *outputSizePtr)
{
  // Resolve output size
  unsigned short outputSize = input->contentSize + 3;

  // Send output size via pointer, if not NULL
  if (outputSizePtr != NULL)
    *outputSizePtr = outputSize;

  // Allocate memory for output
  m2tp_bytes output = (m2tp_bytes)malloc(outputSize);

  // Insert command
  output[0] = input->command;

  // Insert contentSize, as Big Endian bytes
  output[1] = (m2tp_byte)((input->contentSize >> 8) & 0xFFU);
  output[2] = (m2tp_byte)(input->contentSize & 0xFFU);

  // Insert raw content
  for (unsigned short i = 0; i < input->contentSize; i++)
    output[i + 3] = input->content[i];

  return output;
}

void Packet_parse(const m2tp_bytes input, Packet *output)
{
  // FIXME: Technique to check valid .contentSize here

  // Insert command
  output->command = input[0];

  // Insert contentSize
  output->contentSize = 0U;
  output->contentSize |= ((unsigned short)input[1]) << 8;
  output->contentSize |= (unsigned short)input[2];

  // Allocate RAM for content
  output->content = (m2tp_bytes)malloc(output->contentSize);

  // Insert content
  for (unsigned short i = 0; i < output->contentSize; i++)
    output->content[i] = input[i + 3];
}
