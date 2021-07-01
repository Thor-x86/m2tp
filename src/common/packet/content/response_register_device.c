/**
 * @file request_register_device.c
 * @brief Implementation of request_register_device.h
**/

#include "response_register_device.h"

void packet_content_ResponseRegisterDevice_serialize(
    const packet_content_ResponseRegisterDevice *input, m2tp_bytes output, m2tp_byte *outputSizePtr)
{
  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = RESPONSE_REGISTER_DEVICE_SIZE;

  // Insert dice and address to output
  output[0] = input->address;
}

void packet_content_ResponseRegisterDevice_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_ResponseRegisterDevice *output)
{
  // Abort if input size is too small
  if (inputSize < RESPONSE_REGISTER_DEVICE_SIZE)
    return;

  // Insert dice and address to output
  output->address = input[0];
}
