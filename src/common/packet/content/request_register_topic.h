/**
 * @file request_register_topic.h
 * @brief Represents content packet of REQUEST_REGISTER_TOPIC command
**/

#ifndef IS_REQUEST_REGISTER_TOPIC_PACKET_DEFINED
#define IS_REQUEST_REGISTER_TOPIC_PACKET_DEFINED

#include "m2tp-common/typedef.h"

// Data structure of packet content
typedef struct
{

  // Which name of topic?
  char *name;

} packet_content_RequestRegisterTopic;

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern m2tp_bytes packet_content_RequestRegisterTopic_serialize(
    const packet_content_RequestRegisterTopic *input, m2tp_byte *outputSizePtr);

// Convert: Raw Bytes => Packet Object
extern void packet_content_RequestRegisterTopic_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_RequestRegisterTopic *output);

#endif // #ifndef IS_REQUEST_REGISTER_TOPIC_PACKET_DEFINED
