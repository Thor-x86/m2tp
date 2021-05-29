/**
 * @file announcement_quit.h
 * @brief Represents content packet of ANNOUNCEMENT_QUIT command
**/

#ifndef IS_ANNOUNCEMENT_QUIT_PACKET_DEFINED
#define IS_ANNOUNCEMENT_QUIT_PACKET_DEFINED

#include "m2tp-common/typedef.h"

// Data structure of packet content
typedef struct
{

  // Address of quitted device
  m2tp_channel address;

} packet_content_AnnouncementQuit;

// ANNOUNCEMENT_QUIT Packet has fixed size
#define PACKET_CONTENT_SIZE_ANNOUNCEMENT_QUIT 1

// Convert: Packet Object => Raw Bytes
extern m2tp_bytes packet_content_AnnouncementQuit_serialize(
    const packet_content_AnnouncementQuit *packet);

// Convert: Raw Bytes => Packet Object
extern packet_content_AnnouncementQuit packet_content_AnnouncementQuit_parse(
    m2tp_bytes rawBytes);

#endif // #ifndef IS_ANNOUNCEMENT_QUIT_PACKET_DEFINED
