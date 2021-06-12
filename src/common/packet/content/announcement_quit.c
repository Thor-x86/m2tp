/**
 * @file announcement_quit.c
 * @brief Implementation of announcement_quit.h
**/

#include <stdlib.h>

#include "announcement_quit.h"

// ANNOUNCEMENT_QUIT has fixed size
#define _SIZE 1

m2tp_bytes packet_content_AnnouncementQuit_serialize(
    const packet_content_AnnouncementQuit *input, unsigned short *outputSizePtr)
{
  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = _SIZE;

  // Allocate memory for output
  m2tp_bytes output = (m2tp_bytes)malloc(_SIZE);

  // Insert address to output
  output[0] = input->address;

  return output;
}

void packet_content_AnnouncementQuit_parse(
    const m2tp_bytes input, unsigned short inputSize, packet_content_AnnouncementQuit *output)
{
  // Abort if input size is too small
  if (inputSize < _SIZE)
    return;

  // Insert address to output
  output->address = input[0];
}
