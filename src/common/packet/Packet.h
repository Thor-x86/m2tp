/**
 * @file Packet.h
 * @brief Represents each transmission packet
**/

#ifndef IS_PACKET_DEFINED
#define IS_PACKET_DEFINED

#include "m2tp/typedef.h"

// Data structure of each packet
typedef struct Packet
{

  // See "include/commands.h"
  m2tp_byte command;

  // Maximum content size: 255 Bytes
  m2tp_byte contentSize;

  // Serialized packet content
  m2tp_bytes content;

} Packet;

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern void Packet_serialize(const Packet *input, m2tp_bytes output, m2tp_byte *outputSizePtr);

// Convert: Raw Bytes => Packet Object
// WARNING! input must AT LEAST has 3 bytes length
extern void Packet_parse(const m2tp_bytes input, Packet *output);

#endif // #ifndef IS_PACKET_DEFINED
