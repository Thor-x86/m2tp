/**
 * @file request_register_device.c
 * @brief Implementation of request_register_device.h
**/

#include <stdlib.h>

#include "response_register_device.h"

// RESPONSE_REGISTER_DEVICE has fixed size
#define _SIZE 2

m2tp_bytes packet_content_ResponseRegisterDevice_serialize(
    const packet_content_ResponseRegisterDevice *input, unsigned short *outputSizePtr)
{
  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = _SIZE;

  // Allocate memory for output
  m2tp_bytes output = (m2tp_bytes)malloc(_SIZE);

  // Insert dice and address to output
  output[0] = input->dice;
  output[1] = input->address;

  return output;
}

void packet_content_ResponseRegisterDevice_parse(
    const m2tp_bytes input, unsigned short inputSize, packet_content_ResponseRegisterDevice *output)
{
  // Abort if input size is too small
  if (inputSize < _SIZE)
    return;

  // Insert dice and address to output
  output->dice = input[0];
  output->address = input[1];
}
