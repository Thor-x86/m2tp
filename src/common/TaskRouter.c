/**
 * @file TaskRouter.c
 * @brief Implementation of TaskRouter.h
**/

#include "TaskRouter.h"
#include "DeviceState.h"
#include "m2tp-common/base_interface/DriverInterface.h"

m2tp_byte TaskRouter_currentTask = NULL;

void (*TaskRouter_receiveInterrupt)(Packet *packet) = NULL;
void (*TaskRouter_timeoutInterrupt)() = NULL;
void (*TaskRouter_reportReceiveFail)(m2tp_byte errorCode, m2tp_channel where) = NULL;
void (*TaskRouter_reportSendFail)(m2tp_byte errorCode, m2tp_channel where) = NULL;

void TaskRouter_startTimeout(unsigned long milliseconds)
{
  if (m2tp_driver_startTimerListener != NULL && TaskRouter_timeoutInterrupt != NULL)
    m2tp_driver_startTimerListener(milliseconds, TaskRouter_timeoutInterrupt);
}

void TaskRouter_stopTimeout()
{
  if (m2tp_driver_stopTimerListener != NULL)
    m2tp_driver_stopTimerListener();
}

void TaskRouter_clearFunctions()
{
  TaskRouter_stopTimeout();
  TaskRouter_receiveInterrupt = NULL;
  TaskRouter_timeoutInterrupt = NULL;
  TaskRouter_reportReceiveFail = NULL;
  TaskRouter_reportSendFail = NULL;
}

// NOTE: TaskRouter_nextTask and TaskRouter_reset function implemented at member and leader
