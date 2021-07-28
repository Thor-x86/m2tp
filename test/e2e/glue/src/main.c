/**
 * @file main.c
 * @brief Main part of glue layer for E2E test environment
**/

#include "m2tp-virtual.h"

#include "dependencies.h"
#include "variables.h"
#include "threads.h"

//////////////// Private Function Headers ///////////////////////////////////////

void reset();
void onSend(m2tp_byte command, m2tp_byte contentSize, const m2tp_bytes content);
void startTimeout(unsigned long duration, m2tp_driver_TimerFinishCallback callback);
void stopTimeout();

// This is signal handler for SIGUSR1
void onPause(int);

/////////////////////////////////////////////////////////////////////////////////

//////////////// Public Functions ///////////////////////////////////////////////

bool m2tp_connect(unsigned short port, char *deviceClass)
{
  // Initialize variabels
  reset();
  mainThreadID = pthread_self();

  // Configure targeted UDP Socket (Virtual Network)
  vnAddress.sin_family = AF_INET;
  vnAddress.sin_port = htons(port);
  int returnCode = inet_pton(AF_INET, "127.0.0.1", &(vnAddress.sin_addr));
  if (returnCode != 1)
    return false;

  // Configure UDP Socket for current device
  struct sockaddr_in deviceAddress = vnAddress;
  deviceAddress.sin_port = 0;

  // Create UDP Socket
  socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socketDescriptor == -1)
    return false;

  // Make UDP Socket bidirectional
  bind(socketDescriptor,
       (struct sockaddr *)&deviceAddress,
       addressSize);

  // Start the connection
  returnCode = sendto(socketDescriptor,
                      NULL, 0, 0,
                      (struct sockaddr *)&vnAddress,
                      addressSize);
  if (returnCode == -1)
    return false;

  // Assign SIGUSR1 handler for pausing main thread
  // while other thread modifying stuffs to prevent
  // undefined behavior
  {
    struct sigaction signalAction;
    memset(&signalAction, 0, sizeof(struct sigaction));
    signalAction.sa_flags = SA_RESTART;
    signalAction.sa_handler = &onPause;
    sigaction(SIGUSR1, &signalAction, NULL);
  }

  // Configure then start M2TP
  m2tp_driver_setDeviceClass(deviceClass);
  m2tp_driver_sendListener = &onSend;
  m2tp_driver_startTimerListener = &startTimeout;
  m2tp_driver_stopTimerListener = &stopTimeout;
  m2tp_driver_connected();

  // Start ReceiverThread and TimeoutThread
  pthread_create(&receiverThreadID, NULL, &ReceiverThread, NULL);
  pthread_create(&timeoutThreadID, NULL, &TimeoutThread, NULL);

  return true;
}

void m2tp_disconnect()
{
  // Stop timeout and receiver thread
  stopTimeout();
  pthread_cancel(receiverThreadID);
  pthread_cancel(timeoutThreadID);
  pthread_join(receiverThreadID, NULL);
  pthread_join(timeoutThreadID, NULL);

  // For M2TP side
  m2tp_driver_disconnected();
  m2tp_driver_sendListener = NULL;
  m2tp_driver_startTimerListener = NULL;
  m2tp_driver_stopTimerListener = NULL;
  m2tp_driver_setDeviceClass("generic");

  // Close UDP socket gracefully
  shutdown(socketDescriptor, SHUT_RDWR);
  close(socketDescriptor);

  // Reset signal handler
  signal(SIGUSR1, SIG_DFL);

  // Reset variables
  reset();
}

/////////////////////////////////////////////////////////////////////////////////

//////////////// Private Functions //////////////////////////////////////////////

void reset()
{
  socketDescriptor = 0;
  mainThreadID = 0;
  receiverThreadID = 0;
  timeoutThreadID = 0;
  memset(&vnAddress, 0, sizeof(struct sockaddr_in));
}

void onSend(m2tp_byte command, m2tp_byte contentSize, const m2tp_bytes content)
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

  // Send to UDP socket
  ssize_t returnCode = sendto(socketDescriptor,
                              buffer, bufferSize, 0,
                              (struct sockaddr *)&vnAddress,
                              addressSize);

  // Catch error
  if (returnCode == -1)
    m2tp_disconnect();
}

void startTimeout(unsigned long duration,
                  m2tp_driver_TimerFinishCallback callback)
{
  if (pthread_self() != timeoutThreadID)
    pthread_mutex_lock(&timeoutMutex);
  timeout.remaining = duration;
  timeout.callback = callback;
  if (pthread_self() != timeoutThreadID)
    pthread_mutex_unlock(&timeoutMutex);
}

void stopTimeout()
{
  if (pthread_self() != timeoutThreadID)
    pthread_mutex_lock(&timeoutMutex);
  timeout.remaining = 0;
  timeout.callback = NULL;
  if (pthread_self() != timeoutThreadID)
    pthread_mutex_unlock(&timeoutMutex);
}

void onPause(int signalCode)
{
  // Block main thread until other thread
  // unblock it via semaphore
  sem_wait(&mainThreadUnblocker);
}

/////////////////////////////////////////////////////////////////////////////////
