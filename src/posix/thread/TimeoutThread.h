/**
 * @file TimeoutThread.h
 * @brief Runs countdown timer without blocking the app
**/

/*
NOTE: The only reason we don't use kernel-level alarm
      because signal handler won't share global variables
      inside M2TP library with the main thread. So, we
      have to utilize pthread, mutex, semaphore, and simple
      signal handling to solve that problem. Also it is
      impossible to use Shared Memory because M2TP has to
      be available for microcontrollers (MCUs).
*/

#ifndef IS_M2TP_POSIX_TIMEOUTTHREAD_DEFINED
#define IS_M2TP_POSIX_TIMEOUTTHREAD_DEFINED

#include "../dependencies.h"

// Only available when TimeoutThread is running
extern pthread_t TimeoutThread_ID;

// Main loop for timeout thread
extern void *TimeoutThread(void *);

// Start the thread, used in MainThread_attach()
extern void TimeoutThread_init();

// Kill the thread, used in MainThread_detach()
extern void TimeoutThread_destroy();

// Periodically decrement for each millisecond,
// DO NOT touch this directly from other thread
extern volatile unsigned long TimeoutThread_countdown;

// Will be called after countdown reaches 0,
// DO NOT touch this directly from other thread
extern volatile m2tp_driver_TimerFinishCallback TimeoutThread_callback;

// Mutex exists for preventing "Race Condition" problem
extern pthread_mutex_t TimeoutThread_mutex;

// Sets countdown and callback with protection by mutex,
// M2TP core library will call this
extern void TimeoutThread_start(unsigned long duration, m2tp_driver_TimerFinishCallback callback);

// Clear countdown and callback with protection by mutex,
// M2TP core library will call this
extern void TimeoutThread_stop();

#endif // #ifndef IS_M2TP_POSIX_TIMEOUTTHREAD_DEFINED
