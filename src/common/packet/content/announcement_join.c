/**
 * @file announcement_join.c
 * @brief Implementation of announcement_join.h
**/

#include "announcement_join.h"

void packet_content_AnnouncementJoin_serialize(
    const packet_content_AnnouncementJoin *input, m2tp_bytes output, m2tp_byte *outputSizePtr)
{
  // Resolve deviceClass size
  m2tp_byte deviceClassSize = 0;
  for (deviceClassSize; deviceClassSize < 255; deviceClassSize++)
  {
    if (input->deviceClass[deviceClassSize] == '\0')
      break;
  }

  // Resolve output size
  m2tp_byte outputSize = 1 + deviceClassSize;

  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = outputSize;

  // Insert address to output
  output[0] = input->address;

  // Insert deviceClass to output
  for (m2tp_byte i = 0; i < deviceClassSize; i++)
    output[i + 1] = input->deviceClass[i];
}

void packet_content_AnnouncementJoin_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_AnnouncementJoin *output)
{
  // Abort if input size is too small
  if (inputSize < 1)
    return;

  // Insert address to output
  output->address = input[0];

  // Resolve deviceClass size
  m2tp_byte deviceClassSize = inputSize - 1;

  // Insert deviceClass to output
  for (m2tp_byte i = 0; i < deviceClassSize; i++)
    output->deviceClass[i] = input[i + 1];

  // Insert NULL character to deviceClass to prevent segfault error
  output->deviceClass[deviceClassSize] = '\0';
}
