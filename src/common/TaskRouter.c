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
void (*TaskRouter_reportReceiveFail)(m2tp_byte errorCode) = NULL;
void (*TaskRouter_reportSendFail)(m2tp_byte errorCode) = NULL;
void (*TaskRouter_endTask)() = NULL;

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

// NOTE: Other functions are implemented at member and leader
//       because they're work differently
