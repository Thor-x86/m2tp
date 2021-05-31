/**
 * @file response_register_topic.h
 * @brief Represents content packet of RESPONSE_REGISTER_TOPIC command
**/

#ifndef IS_RESPONSE_REGISTER_TOPIC_PACKET_DEFINED
#define IS_RESPONSE_REGISTER_TOPIC_PACKET_DEFINED

#include "m2tp-common/typedef.h"

// Data structure of packet content
typedef struct
{

  // ID of requested topic, use for broadcast
  m2tp_channel ID;

  // Name of requested topic
  char *name;

} packet_content_ResponseRegisterTopic;

// Convert: Packet Object => Raw Bytes
extern m2tp_bytes packet_content_ResponseRegisterTopic_serialize(
    const packet_content_ResponseRegisterTopic *packet);

// Convert: Raw Bytes => Packet Object
extern packet_content_ResponseRegisterTopic packet_content_ResponseRegisterTopic_parse(
    const m2tp_bytes rawBytes);

#endif // #ifndef IS_RESPONSE_REGISTER_TOPIC_PACKET_DEFINED
