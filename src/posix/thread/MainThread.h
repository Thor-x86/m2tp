/**
 * @file MainThread.h
 * @brief Main thread is app's thread
**/

#ifndef IS_M2TP_POSIX_MAINTHREAD_DEFINED
#define IS_M2TP_POSIX_MAINTHREAD_DEFINED

#include "../dependencies.h"

// Only available when MainThread is running
extern pthread_t MainThread_ID;

// Attach M2TP to app's thread,
// call this when ready to connect
extern void MainThread_attach(const char *deviceClass);

// Detach M2TP from app's thread,
// call this before disconnect
extern void MainThread_detach();

// M2TP core library will call this, so you don't have to
extern void MainThread_onSend(m2tp_byte command, m2tp_byte contentSize, const m2tp_bytes content);

// Signal handler going to pause main thread
// while other thread is working, this prevents
// "Race Condition" problem.
//
// WARNING: This is signal handler, DO NOT call!
extern void MainThread_blocker(int signalCode);

// Other thread will use this semaphore to
// resume main thread after it done the job
extern sem_t MainThread_unblocker;

// Only one thread who can pause the main thread,
// otherwise the rest must wait until it resumed,
// this mechanism only possible with mutex
extern pthread_mutex_t MainThread_blockerMutex;

// Helper function to trigger `MainThread_blocker`,
// ONLY CALL THIS FROM OTHER THREAD
extern void MainThread_pause();

// Helper function to utilize `MainThread_unblocker`,
// ONLY CALL THIS FROM OTHER THREAD
extern void MainThread_resume();

#endif // #ifndef IS_M2TP_POSIX_MAINTHREAD_DEFINED
