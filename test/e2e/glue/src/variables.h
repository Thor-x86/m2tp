/**
 * @file variables.h
 * @brief All variables here are shared among threads
**/

#ifndef IS_M2TP_VIRTUAL_GLUE_VARIABLES_DEFINED
#define IS_M2TP_VIRTUAL_GLUE_VARIABLES_DEFINED

#include "dependencies.h"

// Tells the kernel which socket currently in use,
// 0 means no socket is used
extern int socketDescriptor;

// Virtual Network Address
extern struct sockaddr_in vnAddress;

// Required for any functions from <sys/socket.h>
extern socklen_t addressSize;

// Required to use SIGUSR1 which allows main thread pause/resume
extern pthread_t mainThreadID;

// Value 0 means ReceiverThread is not running
extern pthread_t receiverThreadID;

// Value 0 means TimeoutThread is not running
extern pthread_t timeoutThreadID;

// SIGUSR1 pause main thread, semaphore resume it
extern sem_t mainThreadUnblocker;

// Prevents two thread colliding while trying to
// block or unblock main thread
extern pthread_mutex_t mainThreadUnblockerMutex;

// This is where main thread communicate with timeout thread
extern volatile struct Timeout
{
  // In milliseconds
  unsigned long remaining;

  // Came from M2TP internal
  m2tp_driver_TimerFinishCallback callback;
} timeout;

// Prevents memory corruption while start/stop timeout
extern pthread_mutex_t timeoutMutex;

#endif // #ifndef IS_M2TP_VIRTUAL_GLUE_VARIABLES_DEFINED
