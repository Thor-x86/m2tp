#include "dependencies.h"
#include "m2tp-virtual.h"
#include "threads.h"
#include "variables.h"
#include "functions.h"

void *ReceiverThread(void *_)
{
  // Only allow cancel thread while waiting for incoming data
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

  while (true)
  {
    // Allocate stack memory for incoming data
    m2tp_byte buffer[257];

    // Wait until receive something from UDP socket
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    ssize_t returnCode = recvfrom(socketDescriptor,
                                  buffer, 257, 0,
                                  (struct sockaddr *)&vnAddress,
                                  &addressSize);
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    // Catch error
    if (returnCode == -1)
    {
      pauseMainThread();
      m2tp_disconnect();
      resumeMainThread();
      break;
    }

    // Going to use M2TP, need to pause main thread
    pauseMainThread();

    // Assign header
    m2tp_driver_receiveStart(buffer[0], buffer[1]);

    // Assign content
    for (m2tp_byte i = 0; i < buffer[1]; i++)
      m2tp_driver_receiveWrite(buffer[2 + i]);

    // Flush
    m2tp_driver_receiveEnd();

    // It's important to return main thread to normal state
    resumeMainThread();
  }

  receiverThreadID = 0;
  return NULL;
}
