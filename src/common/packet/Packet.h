/**
 * @file Packet.h
 * @brief Represents each transmission packet
**/

#ifndef IS_PACKET_DEFINED
#define IS_PACKET_DEFINED

#include "m2tp-common/typedef.h"

// Data structure of each packet
typedef struct
{

  // See "include/commands.h"
  m2tp_byte command;

  // Maximum content size: 64KB
  unsigned short contentSize;

  // Serialized packet content
  m2tp_bytes content;

} Packet;

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern m2tp_bytes Packet_serialize(const Packet *input, unsigned short *outputSizePtr);

// Convert: Raw Bytes => Packet Object
// WARNING! input must AT LEAST has 3 bytes length
extern void Packet_parse(const m2tp_bytes input, Packet *output);

#endif // #ifndef IS_PACKET_DEFINED
