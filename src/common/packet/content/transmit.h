/**
 * @file transmit.h
 * @brief Represents content packet of TRANSMIT command
**/

#ifndef IS_TRANSMIT_PACKET_DEFINED
#define IS_TRANSMIT_PACKET_DEFINED

#include "m2tp-common/typedef.h"

// Data structure of packet content
typedef struct
{

  // Sender's device address
  m2tp_channel source;

  // Receiver device address or topicID
  m2tp_channel target;

  // Multiple bytes of data, intented for application
  m2tp_bytes data;

  // Won't included on serialization, NULL value if no callback needed
  void (*successCallback)();

  // Won't included on serialization, NULL value if no callback needed
  void (*failedCallback)(m2tp_byte errorCode);

} packet_content_Transmit;

// Convert: Packet Object => Raw Bytes
extern m2tp_bytes packet_content_Transmit_serialize(
    const packet_content_Transmit *packet);

// Convert: Raw Bytes => Packet Object
extern packet_content_Transmit packet_content_Transmit_parse(
    const m2tp_bytes rawBytes);

#endif // #ifndef IS_TRANSMIT_PACKET_DEFINED
