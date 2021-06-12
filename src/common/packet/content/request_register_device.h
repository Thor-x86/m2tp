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
  // just in case two devices register at same time
  m2tp_byte dice;

  // What role of the device?
  char *deviceClass;

} packet_content_RequestRegisterDevice;

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern m2tp_bytes packet_content_RequestRegisterDevice_serialize(
    const packet_content_RequestRegisterDevice *input, m2tp_byte *outputSizePtr);

// Convert: Raw Bytes => Packet Object
extern void packet_content_RequestRegisterDevice_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_RequestRegisterDevice *output);

#endif // #ifndef IS_REQUEST_REGISTER_DEVICE_PACKET_DEFINED
