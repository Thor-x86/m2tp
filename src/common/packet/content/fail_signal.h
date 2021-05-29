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
extern m2tp_bytes packet_content_FailSignal_serialize(
    const packet_content_FailSignal *packet);

// Convert: Raw Bytes => Packet Object
extern packet_content_FailSignal packet_content_FailSignal_parse(
    m2tp_bytes rawBytes);

#endif // #ifndef IS_FAIL_SIGNAL_PACKET_DEFINED
