/**
 * @file UdpServer.c
 * @brief Implementation of UdpServer.h
**/

#include "UdpServer.h"
#include "./variables.h"

#include <stdio.h>

//////// Public Variables /////////////////////////////////

bool UdpServer_enabled = false;

///////////////////////////////////////////////////////////

//////// Private Definition ///////////////////////////////

// We don't need `sin_family`, so we have to craft
// our own data structure, saves 8 by
typedef struct UdpServer_Struct
{
  in_addr_t ipAddress;
  in_port_t udpPort;
} UdpServer_Struct;

///////////////////////////////////////////////////////////

//////// Private Variables ////////////////////////////////

UdpServer_Struct UdpServer_udpAddressList[255];
uint8_t UdpServer_position;
uint8_t UdpServer_size;

///////////////////////////////////////////////////////////

//////// Syntactic Sugars /////////////////////////////////

#define UdpServer_notAllowed (!UdpServer_enabled || connectMode != MODE_SOCKET)

///////////////////////////////////////////////////////////

//////// Public Functions /////////////////////////////////

void UdpServer_clear()
{
  memset(
      &UdpServer_udpAddressList,
      0, sizeof(UdpServer_Struct));
  UdpServer_position = 0;
  UdpServer_size = 0;
}

void UdpServer_add(const struct sockaddr_in *udpAddress)
{
  if UdpServer_notAllowed
    return;

  // Making sure it's a valid UDP address
  if (udpAddress == NULL || udpAddress->sin_family != AF_INET || udpAddress->sin_port == 0 || udpAddress->sin_addr.s_addr == 0)
    return;

  // Prevent duplicate UDP address
  if (UdpServer_check(udpAddress))
    return;

  printf("\tUDP Server: New connection from %d\n", ntohs(udpAddress->sin_port));

  UdpServer_udpAddressList[UdpServer_position].ipAddress = udpAddress->sin_addr.s_addr;
  UdpServer_udpAddressList[UdpServer_position].udpPort = udpAddress->sin_port;

  if (UdpServer_position < 255)
    UdpServer_position++;
  else
    UdpServer_position = 0;

  if (UdpServer_size < 255)
    UdpServer_size++;
}

bool UdpServer_check(const struct sockaddr_in *udpAddress)
{
  for (unsigned int i = 0; i < UdpServer_size; i++)
  {
    UdpServer_Struct each = UdpServer_udpAddressList[i];
    if (udpAddress->sin_addr.s_addr == each.ipAddress && udpAddress->sin_port == each.udpPort)
      return true;
  }

  return false;
}

void UdpServer_broadcast(const void *buffer, size_t bufferSize)
{
  if UdpServer_notAllowed
    return;

  for (unsigned int i = 0; i < UdpServer_size; i++)
  {
    // Get each UDP address from the list
    UdpServer_Struct each = UdpServer_udpAddressList[i];

    // Skip if target UDP address disconnected
    if (each.ipAddress == 0 || each.udpPort == 0)
      continue;

    // Compile back to POSIX standard
    struct sockaddr_in target;
    memset(&target, 0, sizeof(struct sockaddr_in));
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = each.ipAddress;
    target.sin_port = each.udpPort;

    // Send the buffer
    int returnCode = sendto(
        descriptor, buffer, bufferSize, 0,
        (struct sockaddr *)&target,
        sizeof(struct sockaddr_in));

    // Fail? Let's assume target device is gone
    if (returnCode < 0)
      memset(&(UdpServer_udpAddressList[i]), 0, sizeof(UdpServer_Struct));
  }
}

///////////////////////////////////////////////////////////
