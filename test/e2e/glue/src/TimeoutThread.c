#include "dependencies.h"
#include "threads.h"
#include "variables.h"
#include "functions.h"

void *TimeoutThread(void *args)
{
  while (true)
  {
    // Only allow cancel thread while on usleep
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    if (timeout.remaining > 0 && timeout.callback != NULL)
    {
      pthread_mutex_lock(&timeoutMutex);

      timeout.remaining--;

      if (timeout.remaining == 0)
      {
        pauseMainThread();
        timeout.callback();
        resumeMainThread();
      }

      pthread_mutex_unlock(&timeoutMutex);
    }

    // Wait for 1 millisecond
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    usleep(1000);
  }

  timeoutThreadID = 0;
  return NULL;
}
