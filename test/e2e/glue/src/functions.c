#include "dependencies.h"
#include "functions.h"
#include "variables.h"

void pauseMainThread()
{
  // Block other thread which trying to pause main thread
  pthread_mutex_lock(&mainThreadUnblockerMutex);

  // Initialize semaphore for resuming main thread
  sem_init(&mainThreadUnblocker, 0, 0);

  // Now, let's pause the main thread via signal
  pthread_kill(mainThreadID, SIGUSR1);
}

void resumeMainThread()
{
  // Resume main thread via semaphore
  sem_post(&mainThreadUnblocker);

  // Clean semaphore for next use
  sem_destroy(&mainThreadUnblocker);

  // Unblock other thread which trying to pause main thread
  pthread_mutex_unlock(&mainThreadUnblockerMutex);
}
