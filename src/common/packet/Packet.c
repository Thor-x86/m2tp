/**
 * @file Packet.c
 * @brief Implementation of Packet.h
**/

#include <stdlib.h>

#include "Packet.h"

void Packet_serialize(const Packet *input, m2tp_bytes output, m2tp_byte *outputSizePtr)
{
  // Resolve output size
  m2tp_byte outputSize = input->contentSize + 2;

  // Send output size via pointer, if not NULL
  if (outputSizePtr != NULL)
    *outputSizePtr = outputSize;

  // Insert command
  output[0] = input->command;

  // Insert contentSize
  output[1] = input->contentSize;

  // Insert raw content
  for (m2tp_byte i = 0; i < input->contentSize; i++)
    output[i + 2] = input->content[i];
}

void Packet_parse(const m2tp_bytes input, Packet *output)
{
  // FIXME: Technique to check valid .contentSize here

  // Insert command
  output->command = input[0];

  // Insert contentSize
  output->contentSize = input[1];

  // Allocate RAM for content, if not yet
  if (output->content == NULL)
    output->content = (m2tp_bytes)malloc(output->contentSize);

  // Insert content
  for (m2tp_byte i = 0; i < output->contentSize; i++)
    output->content[i] = input[i + 2];
}
