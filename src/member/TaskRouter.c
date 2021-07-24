/**
 * @file TaskRouter.c
 * @brief Implementation of TaskRouter.h, specifically for member
**/

#include "../common/TaskRouter.h"
#include "../common/DeviceState.h"
#include "../common/buffer/ReceiveBuffer.h"
#include "../common/buffer/TransmitBuffer.h"

#include "m2tp-common/commands.h"
#include "task/RegistrationTask.h"
#include "task/MainTask.h"

// Available Tasks
#define TASK_REGISTRATION 1
#define TASK_MAIN 2

void TaskRouter_start()
{
  // Initialize topic subscription
  DeviceState_resetTopics();

  TaskRouter_currentTask = TASK_REGISTRATION;
  RegistrationTask_start();
}

void TaskRouter_nextTask()
{
  switch (TaskRouter_currentTask)
  {

  // Only allow go to the next task from
  // registration task if it actually ready
  case TASK_REGISTRATION:
    if (DeviceState_isReady())
    {
      RegistrationTask_stop();
      TaskRouter_currentTask = TASK_MAIN;
      MainTask_start();
    }
    break;

  case TASK_MAIN:
    MainTask_stop();
    TaskRouter_currentTask = TASK_REGISTRATION;
    RegistrationTask_start();
    break;

  } // switch (TaskRouter_currentTask)
}

void TaskRouter_stop()
{
  switch (TaskRouter_currentTask)
  {

  case TASK_REGISTRATION:
    RegistrationTask_stop();
    break;

  case TASK_MAIN:
    MainTask_stop();
    break;

  } // switch (TaskRouter_currentTask)

  // Reset router
  TaskRouter_currentTask = 0;

  // Reset device state
  DeviceState_assignedAddress = 0;
  DeviceState_driverReady = false;
  DeviceState_resetTopics();

  // Reset buffers
  ReceiveBuffer_abort();
  TransmitBuffer_abort();
}

void TaskRouter_sendPacket(Packet *packet)
{
  MainTask_pendingTransmit = packet;
}

void TaskRouter_sendPacketAsync(
    Packet *packet,
    m2tp_OnSuccessCallback successCallback,
    m2tp_OnErrorCallback failedCallback)
{
  MainTask_pendingTransmit = packet;
  MainTask_onTransmitSuccess = successCallback;
  MainTask_onTransmitFailed = failedCallback;
}

bool TaskRouter_hasPendingData()
{
  return (MainTask_pendingTransmit != NULL);
}

void TaskRouter_assignTopic(char *topicName,
                            m2tp_OnRegisteredCallback registeredCallback,
                            m2tp_TopicListener listener,
                            m2tp_OnErrorCallback failedCallback)
{
  RegistrationTask_pendingTopicName = topicName;
  RegistrationTask_pendingTopicOnSuccess = registeredCallback;
  RegistrationTask_pendingTopicListener = listener;
  RegistrationTask_pendingTopicOnFailed = failedCallback;

  // Stop main task then start registration task
  if (TaskRouter_currentTask == TASK_MAIN)
    TaskRouter_nextTask();
}
