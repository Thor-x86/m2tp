/**
 * @file UdpServer.h
 * @brief Stores UDP infomation of all connected members
 * @warning Only for leader device
**/

#ifndef IS_M2TP_POSIX_MEMBERLIST_DEFINED
#define IS_M2TP_POSIX_MEMBERLIST_DEFINED

#include "dependencies.h"

EXTERN_C_START

// If this not true, then all of UdpServer function do nothing
extern bool UdpServer_enabled;

// Call this everytime UDP server is starting
extern void UdpServer_clear();

// Add new UDP information of a member to the list
extern void UdpServer_add(const struct sockaddr_in *udpAddress);

// Check if member already registered or not
extern bool UdpServer_check(const struct sockaddr_in *udpAddress);

// Send frame to all members which registered in the list
extern void UdpServer_broadcast(const void *buffer, size_t bufferSize);

EXTERN_C_STOP

#endif // #ifndef IS_M2TP_POSIX_MEMBERLIST_DEFINED
