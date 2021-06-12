/**
 * @file fail_signal.c
 * @brief Implementation of fail_signal.h
**/

#include <stdlib.h>

#include "fail_signal.h"

// FAIL_SIGNAL has fixed size
#define _SIZE 1

m2tp_bytes packet_content_FailSignal_serialize(
    const packet_content_FailSignal *input, m2tp_byte *outputSizePtr)
{
  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = _SIZE;

  // Allocate memory for output
  m2tp_bytes output = (m2tp_bytes)malloc(_SIZE);

  // Insert errorCode to output
  output[0] = input->errorCode;

  return output;
}

void packet_content_FailSignal_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_FailSignal *output)
{
  // Abort if input size is too small
  if (inputSize < _SIZE)
    return;

  // Insert errorCode to output
  output->errorCode = input[0];
}
