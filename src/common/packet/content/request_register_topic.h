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
extern m2tp_bytes packet_content_RequestRegisterTopic_serialize(
    const packet_content_RequestRegisterTopic *packet);

// Convert: Raw Bytes => Packet Object
extern packet_content_RequestRegisterTopic packet_content_RequestRegisterTopic_parse(
    m2tp_bytes rawBytes);

#endif // #ifndef IS_REQUEST_REGISTER_TOPIC_PACKET_DEFINED
