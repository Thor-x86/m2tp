/**
 * @file response_register_topic.h
 * @brief Represents content packet of RESPONSE_REGISTER_TOPIC command
**/

#ifndef IS_RESPONSE_REGISTER_TOPIC_PACKET_DEFINED
#define IS_RESPONSE_REGISTER_TOPIC_PACKET_DEFINED

#include "m2tp/typedef.h"

// Data structure of packet content
typedef struct
{

  // ID of requested topic, use for broadcast
  m2tp_channel ID;

} packet_content_ResponseRegisterTopic;

// RESPONSE_REGISTER_TOPIC Packet Content has fixed size
#define RESPONSE_REGISTER_TOPIC_SIZE 1

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern void packet_content_ResponseRegisterTopic_serialize(
    const packet_content_ResponseRegisterTopic *input, m2tp_bytes output, m2tp_byte *outputSizePtr);

// Convert: Raw Bytes => Packet Object
extern void packet_content_ResponseRegisterTopic_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_ResponseRegisterTopic *output);

#endif // #ifndef IS_RESPONSE_REGISTER_TOPIC_PACKET_DEFINED
