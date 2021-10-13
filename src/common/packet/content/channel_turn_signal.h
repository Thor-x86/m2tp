/**
 * @file channel_turn_signal.h
 * @brief Represents content packet of CHANNEL_TURN_SIGNAL command
**/

#ifndef IS_CHANNEL_TURN_SIGNAL_PACKET_DEFINED
#define IS_CHANNEL_TURN_SIGNAL_PACKET_DEFINED

#include "m2tp/typedef.h"

// Data structure of packet content
typedef struct packet_content_ChannelTurnSignal
{

  // Which address or topic allowed to transmit data?
  m2tp_channel channel;

} packet_content_ChannelTurnSignal;

// CHANNEL_TURN_SIGNAL Packet Content has fixed size
#define CHANNEL_TURN_SIGNAL_SIZE 1

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern void packet_content_ChannelTurnSignal_serialize(
    const packet_content_ChannelTurnSignal *input, m2tp_bytes output, m2tp_byte *outputSizePtr);

// Convert: Raw Bytes => Packet Object
extern void packet_content_ChannelTurnSignal_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_ChannelTurnSignal *output);

#endif // #ifndef IS_CHANNEL_TURN_SIGNAL_PACKET_DEFINED
