/**
 * @file announcement_quit.c
 * @brief Implementation of announcement_quit.h
**/

#include "announcement_quit.h"

void packet_content_AnnouncementQuit_serialize(
    const packet_content_AnnouncementQuit *input, m2tp_bytes output, m2tp_byte *outputSizePtr)
{
  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = ANNOUNCEMENT_QUIT_SIZE;

  // Insert address to output
  output[0] = input->address;
}

void packet_content_AnnouncementQuit_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_AnnouncementQuit *output)
{
  // Abort if input size is too small
  if (inputSize < ANNOUNCEMENT_QUIT_SIZE)
    return;

  // Insert address to output
  output->address = input[0];
}
