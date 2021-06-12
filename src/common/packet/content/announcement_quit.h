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

// Convert: Packet Object => Raw Bytes
// returns serialized packet
extern m2tp_bytes packet_content_AnnouncementQuit_serialize(
    const packet_content_AnnouncementQuit *input, m2tp_byte *outputSizePtr);

// Convert: Raw Bytes => Packet Object
extern void packet_content_AnnouncementQuit_parse(
    const m2tp_bytes input, m2tp_byte inputSize, packet_content_AnnouncementQuit *output);

#endif // #ifndef IS_ANNOUNCEMENT_QUIT_PACKET_DEFINED
