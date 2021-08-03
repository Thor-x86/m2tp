/**
 * @file response_register_device.h
 * @brief Represents content packet of RESPONSE_REGISTER_DEVICE command
**/

#ifndef IS_RESPONSE_REGISTER_DEVICE_PACKET_DEFINED
#define IS_RESPONSE_REGISTER_DEVICE_PACKET_DEFINED

#include "m2tp/typedef.h"

// Data structure of packet content
typedef struct
{

  // Current device's new address
  m2tp_channel address;

} packet_content_ResponseRegisterDevice;

// RESPONSE_REGISTER_DEVICE Packet Content has fixed size
#define RESPONSE_REGISTER_DEVICE_SIZE 1

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern void packet_content_ResponseRegisterDevice_serialize(
    const packet_content_ResponseRegisterDevice *input, m2tp_bytes output, m2tp_byte *outputSizePtr);

// Convert: Raw Bytes => Packet Object
extern void packet_content_ResponseRegisterDevice_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_ResponseRegisterDevice *output);

#endif // #ifndef IS_RESPONSE_REGISTER_DEVICE_PACKET_DEFINED
