/**
 * @file ReceiverThread.h
 * @brief Runs countdown timer without blocking the app
**/

#ifndef IS_M2TP_POSIX_RECEIVERTHREAD_DEFINED
#define IS_M2TP_POSIX_RECEIVERTHREAD_DEFINED

#include "../dependencies.h"

// Only available when ReceiverThread is running
extern pthread_t ReceiverThread_ID;

// Main loop for receiver thread
extern void *ReceiverThread(void *);

// Start the thread, used in MainThread_attach()
extern void ReceiverThread_init();

// Kill the thread, used in MainThread_detach()
extern void ReceiverThread_destroy();

#endif // #ifndef IS_M2TP_POSIX_RECEIVERTHREAD_DEFINED
