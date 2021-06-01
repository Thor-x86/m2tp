/**
 * @file channel_turn_signal.h
 * @brief Represents content packet of CHANNEL_TURN_SIGNAL command
**/

#ifndef IS_CHANNEL_TURN_SIGNAL_PACKET_DEFINED
#define IS_CHANNEL_TURN_SIGNAL_PACKET_DEFINED

#include "m2tp-common/typedef.h"

// Data structure of packet content
typedef struct
{

  // Which address or topic allowed to transmit data?
  m2tp_channel channel;

} packet_content_ChannelTurnSignal;

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern m2tp_bytes packet_content_ChannelTurnSignal_serialize(
    const packet_content_ChannelTurnSignal *input, unsigned short *outputSizePtr);

// Convert: Raw Bytes => Packet Object
extern void packet_content_ChannelTurnSignal_parse(
    const m2tp_bytes input, unsigned short inputSize, packet_content_ChannelTurnSignal *output);

#endif // #ifndef IS_CHANNEL_TURN_SIGNAL_PACKET_DEFINED
