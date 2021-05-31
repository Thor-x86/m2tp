/**
 * @file announcement_join.h
 * @brief Represents content packet of ANNOUNCEMENT_JOIN command
**/

#ifndef IS_ANNOUNCEMENT_JOIN_PACKET_DEFINED
#define IS_ANNOUNCEMENT_JOIN_PACKET_DEFINED

#include "m2tp-common/typedef.h"

// Data structure of packet content
typedef struct
{

  // Address of joined device
  m2tp_channel address;

  // Which role represents that device?
  char *deviceClass;

} packet_content_AnnouncementQuit;

// Convert: Packet Object => Raw Bytes
extern m2tp_bytes packet_content_AnnouncementJoin_serialize(
    const packet_content_AnnouncementQuit *packet);

// Convert: Raw Bytes => Packet Object
extern packet_content_AnnouncementQuit packet_content_AnnouncementJoin_parse(
    const m2tp_bytes rawBytes);

#endif // #ifndef IS_ANNOUNCEMENT_JOIN_PACKET_DEFINED
