/**
 * @file fail_signal.h
 * @brief Represents content packet of FAIL_SIGNAL command
**/

#ifndef IS_FAIL_SIGNAL_PACKET_DEFINED
#define IS_FAIL_SIGNAL_PACKET_DEFINED

#include "m2tp-common/typedef.h"

// Data structure of packet content
typedef struct
{

  // See "m2tp-common/errors.h" for details
  m2tp_byte errorCode;

} packet_content_FailSignal;

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern m2tp_bytes packet_content_FailSignal_serialize(
    const packet_content_FailSignal *input, unsigned short *outputSizePtr);

// Convert: Raw Bytes => Packet Object
extern void packet_content_FailSignal_parse(
    const m2tp_bytes input, unsigned short inputSize, packet_content_FailSignal *output);

#endif // #ifndef IS_FAIL_SIGNAL_PACKET_DEFINED
