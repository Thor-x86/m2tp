/**
 * @file channel_turn_signal.c
 * @brief Implementation of channel_turn_signal.h
**/

#include "channel_turn_signal.h"

void packet_content_ChannelTurnSignal_serialize(
    const packet_content_ChannelTurnSignal *input, m2tp_bytes output, m2tp_byte *outputSizePtr)
{
  // Send output size thru pointer, if it isn't null
  if (outputSizePtr != NULL)
    *outputSizePtr = CHANNEL_TURN_SIGNAL_SIZE;

  // Insert channel to output
  output[0] = input->channel;
}

void packet_content_ChannelTurnSignal_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_ChannelTurnSignal *output)
{
  // Abort if input size is too small
  if (inputSize < CHANNEL_TURN_SIGNAL_SIZE)
    return;

  // Insert channel to output
  output->channel = input[0];
}
