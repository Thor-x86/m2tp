/**
 * @file response_register_device.h
 * @brief Represents content packet of RESPONSE_REGISTER_DEVICE command
**/

#ifndef IS_RESPONSE_REGISTER_DEVICE_PACKET_DEFINED
#define IS_RESPONSE_REGISTER_DEVICE_PACKET_DEFINED

#include "m2tp-common/typedef.h"

// Data structure of packet content
typedef struct
{

  // Previously generated random number
  m2tp_byte dice;

  // Current device's new address
  m2tp_channel address;

} packet_content_ResponseRegisterDevice;

// Convert: Packet Object => Raw Bytes
extern m2tp_bytes packet_content_ResponseRegisterDevice_serialize(
    const packet_content_ResponseRegisterDevice *packet);

// Convert: Raw Bytes => Packet Object
extern packet_content_ResponseRegisterDevice packet_content_ResponseRegisterDevice_parse(
    m2tp_bytes rawBytes);

#endif // #ifndef IS_RESPONSE_REGISTER_DEVICE_PACKET_DEFINED
