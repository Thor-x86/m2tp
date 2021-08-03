/**
 * @file request_register_device.h
 * @brief Represents content packet of REQUEST_REGISTER_DEVICE command
**/

#ifndef IS_REQUEST_REGISTER_DEVICE_PACKET_DEFINED
#define IS_REQUEST_REGISTER_DEVICE_PACKET_DEFINED

#include "m2tp/typedef.h"

// Convert: String => Raw Bytes
// returns serialized packet
extern void packet_content_RequestRegisterDevice_serialize(
    const char *deviceClass, m2tp_bytes output, m2tp_byte *outputSizePtr);

// Convert: Raw Bytes => String
extern void packet_content_RequestRegisterDevice_parse(
    const m2tp_bytes input, m2tp_byte inputSize, char *deviceClass);

#endif // #ifndef IS_REQUEST_REGISTER_DEVICE_PACKET_DEFINED
