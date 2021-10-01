/**
 * @file ReceiverThread.c
 * @brief Implementation of ReceiverThread.h
**/

#include "ReceiverThread.h"
#include "m2tp-posix.h"
#include "../dependencies.h"
#include "../variables.h"
#include "./MainThread.h"
#include "../UdpServer.h"

//////////////// Variables ////////////////////////////////

pthread_t ReceiverThread_ID = 0;

///////////////////////////////////////////////////////////

//////////////// Functions ////////////////////////////////

void *ReceiverThread(void *_)
{
  // Prevent cancel thread while initializing,
  // otherwise undefined behavior occurs
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

  // Allocate stack memory for incoming data
  m2tp_byte packet[257];

  // This is only useful if using UDP Server
  struct sockaddr_in sourceUDP;
  socklen_t sourceUDPSize = sizeof(struct sockaddr_in);

  // Non-zero value means incomplete receive
  m2tp_byte remainingBytes = 0;

  // Only useful when packet is fragmented
  unsigned int targetPacketSize = 0;

  while (true)
  {
    ssize_t packetSize = 0;

    // Allow cancel thread while waiting for data
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    // Is hook available?
    if (receiveHook != NULL)
    {
      // Read incoming data as frame
      m2tp_byte frame[maxFrameSize];
      ssize_t frameSize = read(descriptor, frame, maxFrameSize);

      // Don't allow cancel thread while processing data
      pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

      // Process frame to packet with hook
      packetSize = receiveHook(frame, frameSize, packet);
    }

    // ...or no hook?
    else
    {
      // Wait for incoming data
      if (UdpServer_enabled)
        packetSize = recvfrom(
            descriptor, packet, sizeof(packet), 0,
            (struct sockaddr *)&sourceUDP, &sourceUDPSize);
      else
        packetSize = read(descriptor, packet, sizeof(packet));

      // Don't allow cancel thread while processing data
      pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    }

    // Catch error
    if (packetSize < 0)
    {
      MainThread_pause();
      m2tp_disconnect();
      MainThread_resume();
      break;
    }

    // This device running UDP server?
    if (UdpServer_enabled)
    {
      // Add sender UDP information to list
      UdpServer_add(&sourceUDP);
    }

    // Ignore incomplete packet
    // if (packetSize < 2)
    //   continue;

    // We're going to use M2TP core library, to prevent
    // conflict, let's pause the main thread first
    MainThread_pause();

    // Variable i is for pointing (a.k.a index) packet bytes
    unsigned int i = 0;

    // Is previous receive has been completed?
    if (remainingBytes == 0)
    {
      // Assign header
      bool acceptable = m2tp_driver_receiveStart(packet[0], packet[1]);

      targetPacketSize = packet[1] + 2;
      i += 2;

      // Skip receive if header is corrupted
      if (!acceptable)
        continue;
    }

    // The n variable is each splitted content size (or whole if not splitted)
    unsigned int n;

    // Is the packet splitted?
    if (maxPacketSize > 2 && maxPacketSize < 257 && targetPacketSize > maxPacketSize)
    {
      // Is currently the last fragment?
      if (remainingBytes > 0 && remainingBytes < maxPacketSize)
        n = targetPacketSize % maxPacketSize;

      // ...or not last fragment
      else
        n = maxPacketSize;
    }

    // ...or received whole packet at once?
    else
      n = targetPacketSize;

    // Assign content
    for (; i < n; i++)
    {
      remainingBytes = m2tp_driver_receiveWrite(packet[i]);
    }

    // Flush if completed
    if (remainingBytes == 0)
      m2tp_driver_receiveEnd();

    // Don't forget to resume the main thread
    MainThread_resume();
  }

  return NULL;
}

void ReceiverThread_init()
{
  pthread_create(&ReceiverThread_ID, NULL, &ReceiverThread, NULL);
}

void ReceiverThread_destroy()
{
  pthread_cancel(ReceiverThread_ID);
  pthread_join(ReceiverThread_ID, NULL);
  ReceiverThread_ID = 0;
}

///////////////////////////////////////////////////////////
