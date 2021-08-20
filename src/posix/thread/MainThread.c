/**
 * @file MainThread.c
 * @brief Implementation of MainThread.h
**/

#include "MainThread.h"
#include "m2tp-posix.h"
#include "../variables.h"
#include "./ReceiverThread.h"
#include "./TimeoutThread.h"
#include "../UdpServer.h"

//////////////// Variables ////////////////////////////////

pthread_t MainThread_ID = 0;
sem_t MainThread_unblocker;
pthread_mutex_t MainThread_blockerMutex;

///////////////////////////////////////////////////////////

//////////////// Functions ////////////////////////////////

void MainThread_attach(char *deviceClass)
{
  MainThread_ID = pthread_self();
  pthread_mutex_init(&MainThread_blockerMutex, NULL);
  sem_init(&MainThread_unblocker, 0, 0);

  // Submit the assigned signal code to kernel
  {
    struct sigaction signalAction;
    memset(&signalAction, 0, sizeof(struct sigaction));
    signalAction.sa_flags = SA_RESTART;
    signalAction.sa_handler = &MainThread_blocker;
    sigaction(assignedSignalCode, &signalAction, NULL);
  }

  // Start TimeoutThread first, just in case premature callback
  TimeoutThread_init();

  // Configure then start M2TP
  m2tp_driver_setDeviceClass(deviceClass);
  m2tp_driver_sendListener = &MainThread_onSend;
  m2tp_driver_startTimerListener = &TimeoutThread_start;
  m2tp_driver_stopTimerListener = &TimeoutThread_stop;
  m2tp_driver_connected();

  // ReceiverThread possibly call something to M2TP core
  // library right after init, so it need to be started
  // after M2TP hooked to opened file/socket
  ReceiverThread_init();
}

void MainThread_detach()
{
  // All threads must safely shutted down first
  TimeoutThread_destroy();
  ReceiverThread_destroy();

  // For M2TP side
  m2tp_driver_disconnected();
  m2tp_driver_sendListener = NULL;
  m2tp_driver_startTimerListener = NULL;
  m2tp_driver_stopTimerListener = NULL;
  m2tp_driver_setDeviceClass("generic");

  signal(assignedSignalCode, SIG_DFL);
  pthread_mutex_destroy(&MainThread_blockerMutex);
  sem_destroy(&MainThread_unblocker);
  MainThread_ID = 0;
}

void MainThread_pause()
{
  // Prevents multiple threads pause Main Thread
  // at same time, which also cause Race Condition
  pthread_mutex_lock(&MainThread_blockerMutex);

  // Now, let's pause the main thread via signal
  pthread_kill(MainThread_ID, assignedSignalCode);
}

void MainThread_resume()
{
  // Resume main thread via semaphore
  sem_post(&MainThread_unblocker);

  // Let other thread pause Main Thread again
  pthread_mutex_unlock(&MainThread_blockerMutex);
}

void MainThread_onSend(
    m2tp_byte command,
    m2tp_byte contentSize,
    const m2tp_bytes content)
{
  // Allocate stack memory
  size_t bufferSize = contentSize + 2;
  m2tp_byte buffer[bufferSize];

  // Put the header
  buffer[0] = command;
  buffer[1] = contentSize;

  // Put the content
  for (m2tp_byte i = 0; i < contentSize; i++)
    buffer[2 + i] = content[i];

  // Is current device both leader and using UDP?
  if (UdpServer_enabled)
    // Use UDP server to transmit
    UdpServer_broadcast(buffer, bufferSize);

  // ...or not?
  else
  {
    // Send to network via syscall
    ssize_t returnCode = write(descriptor, buffer, bufferSize);

    // Catch error
    if (returnCode < 0)
      m2tp_disconnect();
  }
}

void MainThread_blocker(int signalCode)
{
  // After signal with code from `usingSignalCode`
  // variable handled, block main thread until
  // being unblocked by other thread via semaphore
  sem_wait(&MainThread_unblocker);
}

///////////////////////////////////////////////////////////
