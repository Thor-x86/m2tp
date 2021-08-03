/**
 * @file request_register_topic.h
 * @brief Represents content packet of REQUEST_REGISTER_TOPIC command
**/

#ifndef IS_REQUEST_REGISTER_TOPIC_PACKET_DEFINED
#define IS_REQUEST_REGISTER_TOPIC_PACKET_DEFINED

#include "m2tp/typedef.h"

// Convert: String => Raw Bytes
// returns serialized packet
extern void packet_content_RequestRegisterTopic_serialize(
    const char *topicName, m2tp_bytes output, m2tp_byte *outputSizePtr);

// Convert: Raw Bytes => String
extern void packet_content_RequestRegisterTopic_parse(
    const m2tp_bytes input, m2tp_byte inputSize, char *topicName);

#endif // #ifndef IS_REQUEST_REGISTER_TOPIC_PACKET_DEFINED
