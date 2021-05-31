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

// CHANNEL_TURN_SIGNAL Packet has fixed size
#define PACKET_CONTENT_SIZE_CHANNEL_TURN_SIGNAL 1

// Convert: Packet Object => Raw Bytes
extern m2tp_bytes packet_content_ChannelTurnSignal_serialize(
    const packet_content_ChannelTurnSignal *packet);

// Convert: Raw Bytes => Packet Object
extern packet_content_ChannelTurnSignal packet_content_ChannelTurnSignal_parse(
    const m2tp_bytes rawBytes);

#endif // #ifndef IS_CHANNEL_TURN_SIGNAL_PACKET_DEFINED
