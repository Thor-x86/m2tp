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
    if (packetSize < 2)
      continue;

    // We're going to use M2TP core library, to prevent
    // conflict, let's pause the main thread first
    MainThread_pause();

    // Assign header
    m2tp_driver_receiveStart(packet[0], packet[1]);

    // Assign content
    for (m2tp_byte i = 0; i < packet[1]; i++)
      m2tp_driver_receiveWrite(packet[2 + i]);

    // Flush
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
