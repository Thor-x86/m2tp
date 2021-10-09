/**
 * @file TaskRouter.c
 * @brief Implementation of TaskRouter.h
**/

#include "TaskRouter.h"
#include "DeviceState.h"
#include "m2tp/interface/driver.h"

volatile m2tp_byte TaskRouter_currentTask = 0;
volatile unsigned long TaskRouter_timeoutDuration = 0;

void (*TaskRouter_receiveInterrupt)(Packet *packet) = NULL;
void (*TaskRouter_timeoutInterrupt)() = NULL;

void TaskRouter_startTimeout(unsigned long milliseconds)
{
  if (m2tp_driver_startTimerListener != NULL && TaskRouter_timeoutInterrupt != NULL)
    m2tp_driver_startTimerListener(milliseconds, TaskRouter_timeoutInterrupt);

  TaskRouter_timeoutDuration = milliseconds;
}

void TaskRouter_restartTimeout()
{
  if (m2tp_driver_stopTimerListener != NULL)
    m2tp_driver_stopTimerListener();

  if (m2tp_driver_startTimerListener != NULL && TaskRouter_timeoutInterrupt != NULL)
    m2tp_driver_startTimerListener(TaskRouter_timeoutDuration, TaskRouter_timeoutInterrupt);
}

void TaskRouter_stopTimeout()
{
  if (m2tp_driver_stopTimerListener != NULL)
    m2tp_driver_stopTimerListener();

  TaskRouter_timeoutDuration = 0;
}

// NOTE: Other functions are implemented at member and leader
//       because they're work differently
