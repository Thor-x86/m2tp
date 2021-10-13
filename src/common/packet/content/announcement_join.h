/**
 * @file announcement_join.h
 * @brief Represents content packet of ANNOUNCEMENT_JOIN command
**/

#ifndef IS_ANNOUNCEMENT_JOIN_PACKET_DEFINED
#define IS_ANNOUNCEMENT_JOIN_PACKET_DEFINED

#include "m2tp/typedef.h"

// Data structure of packet content
typedef struct packet_content_AnnouncementJoin
{

  // Address of joined device
  m2tp_channel address;

  // Which role represents that device?
  char *deviceClass;

} packet_content_AnnouncementJoin;

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern void packet_content_AnnouncementJoin_serialize(
    const packet_content_AnnouncementJoin *input, m2tp_bytes output, m2tp_byte *outputSizePtr);

// Convert: Raw Bytes => Packet Object
extern void packet_content_AnnouncementJoin_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_AnnouncementJoin *output);

#endif // #ifndef IS_ANNOUNCEMENT_JOIN_PACKET_DEFINED
