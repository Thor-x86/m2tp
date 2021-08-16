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
  m2tp_byte buffer[257];

  // This is only useful if using UDP Server
  struct sockaddr_in sourceUDP;
  socklen_t sourceUDPSize = sizeof(struct sockaddr_in);

  while (true)
  {
    // Allow cancel thread while waiting for data
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    // Wait for incoming data
    ssize_t returnCode = 0;
    if (UdpServer_enabled)
      returnCode = recvfrom(
          descriptor, buffer, sizeof(buffer), 0,
          (struct sockaddr *)&sourceUDP, &sourceUDPSize);
    else
      returnCode = read(descriptor, buffer, sizeof(buffer));

    // Don't allow cancel thread while processing data,
    // we're going to play with mutex and semaphore
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    // Catch error
    if (returnCode < 0)
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

    // Ignore incomplete frame
    if (returnCode < 2)
      continue;

    // We're going to use M2TP core library, to prevent
    // conflict, let's pause the main thread first
    MainThread_pause();

    // Assign header
    m2tp_driver_receiveStart(buffer[0], buffer[1]);

    // Assign content
    for (m2tp_byte i = 0; i < buffer[1]; i++)
      m2tp_driver_receiveWrite(buffer[2 + i]);

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
