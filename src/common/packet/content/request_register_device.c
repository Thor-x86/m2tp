/**
 * @file request_register_device.c
 * @brief Implementation of request_register_device.h
**/

#include <stdlib.h>

#include "request_register_device.h"

void packet_content_RequestRegisterDevice_serialize(
    const char *deviceClass, m2tp_bytes output, m2tp_byte *outputSizePtr)
{
  // Resolve deviceClass size
  m2tp_byte deviceClassSize = 0;
  for (deviceClassSize; deviceClassSize < 255; deviceClassSize++)
  {
    if (deviceClass[deviceClassSize] == '\0')
      break;
  }

  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = deviceClassSize;

  // Insert deviceClass to output
  for (m2tp_byte i = 0; i < deviceClassSize; i++)
    output[i] = deviceClass[i];
}

void packet_content_RequestRegisterDevice_parse(
    const m2tp_bytes input, m2tp_byte inputSize, char *deviceClass)
{
  // Insert deviceClass to output
  for (m2tp_byte i = 0; i < inputSize; i++)
    deviceClass[i] = input[i];

  // Insert NULL character to deviceClass to prevent segfault error
  deviceClass[inputSize] = '\0';
}
