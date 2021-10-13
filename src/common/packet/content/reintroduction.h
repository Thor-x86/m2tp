/**
 * @file reintroduction.h
 * @brief Represents content packet of REINTRODUCTION command
**/

#ifndef IS_REINTRODUCTION_PACKET_DEFINED
#define IS_REINTRODUCTION_PACKET_DEFINED

#include "m2tp/typedef.h"

// Data structure of packet content
typedef struct packet_content_Reintroduction
{

  // Address of already joined device
  m2tp_channel address;

  // Which role represents that device?
  char *deviceClass;

} packet_content_Reintroduction;

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern void packet_content_Reintroduction_serialize(
    const packet_content_Reintroduction *input, m2tp_bytes output, m2tp_byte *outputSizePtr);

// Convert: Raw Bytes => Packet Object
extern void packet_content_Reintroduction_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_Reintroduction *output);

#endif // #ifndef IS_REINTRODUCTION_PACKET_DEFINED
