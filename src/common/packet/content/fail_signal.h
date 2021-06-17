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

// FAIL_SIGNAL Packet Content has fixed size
#define FAIL_SIGNAL_SIZE 1

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern void packet_content_FailSignal_serialize(
    const packet_content_FailSignal *input, m2tp_bytes output, m2tp_byte *outputSizePtr);

// Convert: Raw Bytes => Packet Object
extern void packet_content_FailSignal_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_FailSignal *output);

#endif // #ifndef IS_FAIL_SIGNAL_PACKET_DEFINED
