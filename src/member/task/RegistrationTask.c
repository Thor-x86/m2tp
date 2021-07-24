/**
 * @file RegistrationTask.c
 * @brief Implementation of RegistrationTask.h
**/

#include "RegistrationTask.h"

#include <stdlib.h>

#include "m2tp-common/commands.h"
#include "m2tp-member/DriverInterface.h"

#include "../../common/DeviceState.h"
#include "../../common/TaskRouter.h"
#include "../../common/Flag.h"
#include "../../common/packet/Packet.h"
#include "../../common/packet/content/request_register_device.h"
#include "../../common/packet/content/request_register_topic.h"
#include "../../common/packet/content/response_register_device.h"
#include "../../common/packet/content/response_register_topic.h"

//////// Configurations ////////////////////////////////

#define RegistrationTask_RANDELAY_MAX 500ULL
#define RegistrationTask_TIMEOUT 10

////////////////////////////////////////////////////////

//////// Available Flags ///////////////////////////////

// TRUE when currently waiting randomized
// delay for device registration
#define RegistrationTask_FLAG_RANDELAY_DEVICE 0

// TRUE when currently waiting randomized
// delay for topic subscription
#define RegistrationTask_FLAG_RANDELAY_TOPIC 1

// TRUE after requested for device registration
#define RegistrationTask_FLAG_REQUESTED_DEVICE 2

// TRUE after requested for topic subscription
#define RegistrationTask_FLAG_REQUESTED_TOPIC 3

////////////////////////////////////////////////////////

//////// Public Variables //////////////////////////////

char *RegistrationTask_pendingTopicName = NULL;
m2tp_OnRegisteredCallback RegistrationTask_pendingTopicOnSuccess = NULL;
m2tp_OnErrorCallback RegistrationTask_pendingTopicOnFailed = NULL;
m2tp_TopicListener RegistrationTask_pendingTopicListener = NULL;

////////////////////////////////////////////////////////

//////// Private Variables /////////////////////////////

m2tp_byte RegistrationTask_flags = 0;

////////////////////////////////////////////////////////

//////// Syntactic Sugars //////////////////////////////

#define RegistrationTask_generateDelay() (rand() * RegistrationTask_RANDELAY_MAX / RAND_MAX)
#define RegistrationTask_hasPendingSubscribe() (RegistrationTask_pendingTopicName != NULL)

////////////////////////////////////////////////////////

//////// Private Functions /////////////////////////////

void RegistrationTask_resetPendingSubscribe()
{
  RegistrationTask_pendingTopicName = NULL;
  RegistrationTask_pendingTopicOnSuccess = NULL;
  RegistrationTask_pendingTopicOnFailed = NULL;
  RegistrationTask_pendingTopicListener = NULL;
}

void RegistrationTask_receiveInterrupt(Packet *packet)
{
  switch (packet->command)
  {

  case M2TP_COMMAND_REGISTRATION_SIGNAL:
  {
    // Stop timeout
    TaskRouter_stopTimeout();

    // am I registered?
    if (DeviceState_isReady())
    {
      // Has pending subscription?
      if (RegistrationTask_hasPendingSubscribe())
      {
        // Set flag RANDELAY_TOPIC
        Flag_set(RegistrationTask_flags, RegistrationTask_FLAG_RANDELAY_TOPIC);

        // Generate random delay
        unsigned int randelay = RegistrationTask_generateDelay();

        // Start timeout
        TaskRouter_startTimeout(randelay);
      }

      // ...or no pending subscription yet?
      else
      {
        // Stop current task then go to the next one
        TaskRouter_nextTask();
        return;
      }
    }

    // ...or not registered yet?
    else
    {
      // Set flag RANDELAY_ADDRESS
      Flag_set(RegistrationTask_flags, RegistrationTask_FLAG_RANDELAY_DEVICE);

      // Generate random delay
      unsigned int randelay = RegistrationTask_generateDelay();

      // Start timeout
      TaskRouter_startTimeout(randelay);
    }
  }
  break;

  case M2TP_COMMAND_REQUEST_REGISTER_DEVICE:
  case M2TP_COMMAND_REQUEST_REGISTER_TOPIC:
  {
    // Stop timeout
    TaskRouter_stopTimeout();

    // Reset task
    RegistrationTask_resetPendingSubscribe();
    RegistrationTask_flags = 0;
  }
  break;

  case M2TP_COMMAND_RESPONSE_REGISTER_DEVICE:
  {
    // Stop timeout
    TaskRouter_stopTimeout();

    // did I request for device register?
    if (Flag_check(RegistrationTask_flags, RegistrationTask_FLAG_REQUESTED_DEVICE))
    {
      // Unset REQUESTED_ADDRESS flag
      Flag_unset(RegistrationTask_flags, RegistrationTask_FLAG_REQUESTED_DEVICE);

      // Parse the packet content
      packet_content_ResponseRegisterDevice content;
      packet_content_ResponseRegisterDevice_parse(packet->content, packet->contentSize, &content);

      // Assign the address
      DeviceState_assignedAddress = content.address;

      // Stop current task then go to the next one
      TaskRouter_nextTask();
      return;
    }

    // ...or not request yet?
    else
    {
      // Reset task
      RegistrationTask_resetPendingSubscribe();
      RegistrationTask_flags = 0;
    }
  }
  break;

  case M2TP_COMMAND_RESPONSE_REGISTER_TOPIC:
  {
    // Stop timeout
    TaskRouter_stopTimeout();

    // did I request for topic subscription?
    if (Flag_check(RegistrationTask_flags, RegistrationTask_FLAG_REQUESTED_TOPIC))
    {
      // Unset REQUESTED_TOPIC flag
      Flag_unset(RegistrationTask_flags, RegistrationTask_FLAG_REQUESTED_TOPIC);

      // Parse the packet content
      packet_content_ResponseRegisterTopic content;
      packet_content_ResponseRegisterTopic_parse(packet->content, packet->contentSize, &content);

      // Subscribe listener to topic ID, if available
      if (RegistrationTask_pendingTopicListener != NULL)
        DeviceState_assignTopic(content.ID, RegistrationTask_pendingTopicListener);

      // Call topic creation success callback, if exist
      if (RegistrationTask_pendingTopicOnSuccess != NULL)
        RegistrationTask_pendingTopicOnSuccess(content.ID);

      // Stop current task then go to the next one
      TaskRouter_nextTask();
      return;
    }

    // ...or not request yet?
    else
    {
      // Reset task
      RegistrationTask_resetPendingSubscribe();
      RegistrationTask_flags = 0;
    }
  }
  break;

  } // switch (packet->command)
}

void RegistrationTask_timeoutInterrupt()
{
  // If REQUESTED flags set TRUE?
  if (Flag_check(RegistrationTask_flags, RegistrationTask_FLAG_REQUESTED_DEVICE) || Flag_check(RegistrationTask_flags, RegistrationTask_FLAG_REQUESTED_TOPIC))
  {
    // Reset task
    RegistrationTask_resetPendingSubscribe();
    RegistrationTask_flags = 0;
  }

  // ...or RANDELAY_DEVICE flag set TRUE?
  else if (Flag_check(RegistrationTask_flags, RegistrationTask_FLAG_RANDELAY_DEVICE))
  {
    // Set REQUESTED_DEVICE flag
    Flag_set(RegistrationTask_flags, RegistrationTask_FLAG_REQUESTED_DEVICE);

    // Is it safe to send?
    if (m2tp_driver_sendListener != NULL)
    {
      // Serialize device class to raw data
      m2tp_byte serializedContent[255];
      m2tp_byte serializedSize = 0;
      packet_content_RequestRegisterDevice_serialize(DeviceState_deviceClass, serializedContent, &serializedSize);

      // Send request register device
      m2tp_driver_sendListener(M2TP_COMMAND_REQUEST_REGISTER_DEVICE, serializedSize, serializedContent);
    }
  }

  // ...or RANDELAY_TOPIC flag set TRUE?
  else if (Flag_check(RegistrationTask_flags, RegistrationTask_FLAG_RANDELAY_TOPIC))
  {
    // Set REQUESTED_TOPIC flag
    Flag_set(RegistrationTask_flags, RegistrationTask_FLAG_REQUESTED_TOPIC);

    // Is it safe to send?
    if (m2tp_driver_sendListener != NULL && RegistrationTask_hasPendingSubscribe())
    {
      // Serialize topic name to raw data
      m2tp_byte serializedContent[255];
      m2tp_byte serializedSize = 0;
      packet_content_RequestRegisterTopic_serialize(RegistrationTask_pendingTopicName, serializedContent, &serializedSize);

      // Send request register topic
      m2tp_driver_sendListener(M2TP_COMMAND_REQUEST_REGISTER_TOPIC, serializedSize, serializedContent);
    }
  }

  // ...or all flags still unset?
  else
  {
    // Reset task
    RegistrationTask_resetPendingSubscribe();
    RegistrationTask_flags = 0;
  }
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void RegistrationTask_start()
{
  // Assign functions to router
  TaskRouter_receiveInterrupt = &RegistrationTask_receiveInterrupt;
  TaskRouter_timeoutInterrupt = &RegistrationTask_timeoutInterrupt;
}

void RegistrationTask_stop()
{
  // Stop timeout
  TaskRouter_stopTimeout();

  // Detach functions
  TaskRouter_receiveInterrupt = NULL;
  TaskRouter_timeoutInterrupt = NULL;

  // Reset task
  RegistrationTask_resetPendingSubscribe();
  RegistrationTask_flags = 0;
}

////////////////////////////////////////////////////////
