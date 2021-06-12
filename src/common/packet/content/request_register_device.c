/**
 * @file request_register_device.c
 * @brief Implementation of request_register_device.h
**/

#include <stdlib.h>

#include "request_register_device.h"

m2tp_bytes packet_content_RequestRegisterDevice_serialize(
    const packet_content_RequestRegisterDevice *input, m2tp_byte *outputSizePtr)
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

  // Allocate memory with fixed size
  m2tp_bytes output = (m2tp_bytes)malloc(outputSize);

  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = outputSize;

  // Insert dice number to output
  output[0] = input->dice;

  // Insert deviceClass to output
  for (m2tp_byte i = 0; i < deviceClassSize; i++)
    output[i + 1] = input->deviceClass[i];

  return output;
}

void packet_content_RequestRegisterDevice_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_RequestRegisterDevice *output)
{
  // Abort if input size is too small
  if (inputSize < 1)
    return;

  // Insert dice number to output
  output->dice = input[0];

  // Resolve deviceClass size, then allocate
  m2tp_byte deviceClassSize = inputSize - 1;
  output->deviceClass = (char *)malloc(deviceClassSize + 1);

  // Insert deviceClass to output
  for (m2tp_byte i = 0; i < deviceClassSize; i++)
    output->deviceClass[i] = input[i + 1];

  // Insert NULL character to deviceClass to prevent segfault error
  output->deviceClass[deviceClassSize] = '\0';
}
