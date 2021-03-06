/**
 * @file TaskRouter.c
 * @brief Implementation of TaskRouter.h, specifically for leader
**/

#include "../common/TaskRouter.h"
#include "../common/DeviceState.h"

#include <string.h>

#include "m2tp/errors.h"
#include "m2tp/commands.h"

#include "NetworkState.h"
#include "task/RegistrationTask.h"
#include "task/TransmitTask.h"
#include "task/TrafficTask.h"
#include "../common/buffer/TransmitBuffer.h"
#include "../common/buffer/ReceiveBuffer.h"

// Available Tasks
#define TASK_REGISTRATION 1
#define TASK_TRANSMIT 2
#define TASK_TRAFFIC 3

void TaskRouter_start()
{
  // Initialize topic subscription
  DeviceState_resetTopics();

  // Initialize the network
  NetworkState_init();

  TaskRouter_currentTask = TASK_REGISTRATION;
  RegistrationTask_start();
}

void TaskRouter_nextTask()
{
  switch (TaskRouter_currentTask)
  {

  case TASK_REGISTRATION:
  {
    RegistrationTask_stop();
    TaskRouter_currentTask = TASK_TRANSMIT;
    TransmitTask_start();
  }
  break;

  case TASK_TRANSMIT:
  {
    TransmitTask_stop();
    TaskRouter_currentTask = TASK_TRAFFIC;
    TrafficTask_start();
  }
  break;

  case TASK_TRAFFIC:
  {
    TrafficTask_stop();
    TaskRouter_currentTask = TASK_REGISTRATION;
    RegistrationTask_start();
  }
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

  case TASK_TRANSMIT:
    TransmitTask_stop();
    break;

  case TASK_TRAFFIC:
    TrafficTask_stop();
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

  // Clean up network registries
  NetworkState_reset();
}

void TaskRouter_sendPacket(Packet *packet)
{
  TransmitTask_pendingTransmit = packet;
}

void TaskRouter_sendPacketAsync(
    Packet *packet,
    m2tp_OnSuccessCallback successCallback,
    m2tp_OnErrorCallback failedCallback)
{
  TransmitTask_pendingTransmit = packet;
  TransmitTask_onTransmitSuccess = successCallback;
  TransmitTask_onTransmitFailed = failedCallback;
}

volatile bool TaskRouter_hasPendingData()
{
  return (TransmitTask_pendingTransmit != NULL);
}

void TaskRouter_assignTopic(char *topicName,
                            m2tp_OnRegisteredCallback registeredCallback,
                            m2tp_TopicListener listener,
                            m2tp_OnErrorCallback failedCallback)
{
  // Abort if device not ready yet
  if (!DeviceState_isReady())
  {
    if (failedCallback != NULL)
      failedCallback(M2TP_ERROR_NOT_CONNECTED);
    return;
  }

  // Get existing topic ID, if exist
  m2tp_channel topicID = NetworkState_findTopic(topicName);

  // Is topic name not exist yet?
  if (topicID == 0)
  {
    // Is topic registry full?
    if (NetworkState_isTopicFull)
    {
      // Send error callback
      if (failedCallback != NULL)
        failedCallback(M2TP_ERROR_TOO_MUCH_TOPIC);
    }

    // ...or there's still unused topic ID?
    else
    {
      // Register the topic
      unsigned int topicNameSize = strlen(topicName);
      NetworkState_assign(NetworkState_nextVacantTopicID, topicName, topicNameSize);

      // Assign topic listener
      if (listener != NULL)
        DeviceState_assignTopic(NetworkState_nextVacantTopicID, listener);

      // Send success callback
      if (registeredCallback != NULL)
        registeredCallback(NetworkState_nextVacantTopicID);

      // Resolve the next vacant topic ID
      NetworkState_resolveNextVacantTopicID();
    }
  }

  // ...or topic name already exist?
  else
  {
    // Send success callback
    if (registeredCallback != NULL)
      registeredCallback(topicID);
  }
}
