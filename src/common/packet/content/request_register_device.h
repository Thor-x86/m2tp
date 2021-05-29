/**
 * @file request_register_device.h
 * @brief Represents content packet of REQUEST_REGISTER_DEVICE command
**/

#ifndef IS_REQUEST_REGISTER_DEVICE_PACKET_DEFINED
#define IS_REQUEST_REGISTER_DEVICE_PACKET_DEFINED

#include "m2tp-common/typedef.h"

// Data structure of packet content
typedef struct
{

  // Random number 0-255,
  // just in case two device register at same time
  m2tp_byte dice;

  // What role of the device?
  char *deviceClass;

} packet_content_RequestRegisterDevice;

// Convert: Packet Object => Raw Bytes
extern m2tp_bytes packet_content_RequestRegisterDevice_serialize(
    const packet_content_RequestRegisterDevice *packet);

// Convert: Raw Bytes => Packet Object
extern packet_content_RequestRegisterDevice packet_content_RequestRegisterDevice_parse(
    m2tp_bytes rawBytes);

#endif // #ifndef IS_REQUEST_REGISTER_DEVICE_PACKET_DEFINED
