/**
 * @file TimeoutThread.c
 * @brief Implementation of TimeoutThread.h
**/

#include "TimeoutThread.h"
#include "./MainThread.h"

//////////////// Variables ////////////////////////////////

pthread_t TimeoutThread_ID = 0;
volatile unsigned long TimeoutThread_countdown;
volatile m2tp_driver_TimerFinishCallback TimeoutThread_callback;
pthread_mutex_t TimeoutThread_mutex;

///////////////////////////////////////////////////////////

//////////////// Functions ////////////////////////////////

void *TimeoutThread(void *_)
{
  while (true)
  {
    // Only allow cancel thread while on usleep,
    // because we're going to use mutex
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    // Prevent "Race Condition" with TimeoutThread_start
    // and TimeoutThread_stop
    pthread_mutex_lock(&TimeoutThread_mutex);

    if (TimeoutThread_countdown > 0)
    {
      TimeoutThread_countdown--;
      if (TimeoutThread_countdown == 0 && TimeoutThread_callback != NULL)
      {
        MainThread_pause();
        TimeoutThread_callback();
        MainThread_resume();
      }
    }

    // Remember to always restore the mutex
    pthread_mutex_unlock(&TimeoutThread_mutex);

    // Wait for 1 millisecond
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    usleep(1000);
  }

  return NULL;
}

void TimeoutThread_init()
{
  TimeoutThread_countdown = 0;
  TimeoutThread_callback = NULL;
  pthread_mutex_init(&TimeoutThread_mutex, NULL);
  pthread_create(&TimeoutThread_ID, NULL, &TimeoutThread, NULL);
}

void TimeoutThread_destroy()
{
  TimeoutThread_stop();
  pthread_cancel(TimeoutThread_ID);
  pthread_join(TimeoutThread_ID, NULL);
  pthread_mutex_destroy(&TimeoutThread_mutex);
  TimeoutThread_ID = 0;
}

void TimeoutThread_start(
    unsigned long duration,
    m2tp_driver_TimerFinishCallback callback)
{
  pthread_t currentThread = pthread_self();

  // Remember, timeout thread can start countdown itself
  // thru M2TP core library. If we lock the mutex under
  // timeout thread, implicitly we lock the mutex twice.
  // Thus, the process will simply stuck.
  if (currentThread != TimeoutThread_ID)
    pthread_mutex_lock(&TimeoutThread_mutex);

  TimeoutThread_countdown = duration;
  TimeoutThread_callback = callback;

  if (currentThread != TimeoutThread_ID)
    pthread_mutex_unlock(&TimeoutThread_mutex);
}

void TimeoutThread_stop()
{
  pthread_t currentThread = pthread_self();

  // Timeout thread can also stop countdown itself
  if (currentThread != TimeoutThread_ID)
    pthread_mutex_lock(&TimeoutThread_mutex);

  TimeoutThread_countdown = 0;
  TimeoutThread_callback = NULL;

  if (currentThread != TimeoutThread_ID)
    pthread_mutex_unlock(&TimeoutThread_mutex);
}

///////////////////////////////////////////////////////////
