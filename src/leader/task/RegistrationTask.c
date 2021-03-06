/**
 * @file RegistrationTask.c
 * @brief Implementation of RegistrationTask.h
**/

#include "RegistrationTask.h"

#include "m2tp/commands.h"
#include "m2tp/errors.h"
#include "m2tp/interface/driver.h"

#include "../NetworkState.h"
#include "../../common/TaskRouter.h"
#include "../../common/DeviceState.h"
#include "../../common/packet/Packet.h"
#include "../../common/packet/content/fail_signal.h"
#include "../../common/packet/content/request_register_device.h"
#include "../../common/packet/content/request_register_topic.h"
#include "../../common/packet/content/response_register_device.h"
#include "../../common/packet/content/response_register_topic.h"
#include "../../common/packet/content/reintroduction.h"
#include "../../common/packet/content/announcement_join.h"

//////// Configurations ////////////////////////////////

#define RegistrationTask_TIMEOUT 500 // milliseconds

////////////////////////////////////////////////////////

void RegistrationTask_receiveInterrupt(Packet *packet)
{
  // Stop timeout
  TaskRouter_stopTimeout();

  switch (packet->command)
  {

  case M2TP_COMMAND_REQUEST_REGISTER_DEVICE:
  {
    // Is address full?
    if (NetworkState_isAddressFull)
    {
      // Is send listener ready?
      if (m2tp_driver_sendListener != NULL)
      {
        // Create packet content for fail signal
        packet_content_FailSignal failSignalContent;
        failSignalContent.errorCode = M2TP_ERROR_TOO_MUCH_ADDRESS;

        // Serialize that packet content
        m2tp_byte serializedContent[FAIL_SIGNAL_SIZE];
        packet_content_FailSignal_serialize(&failSignalContent, serializedContent, NULL);

        // Send FAIL SIGNAL
        m2tp_driver_sendListener(M2TP_COMMAND_FAIL_SIGNAL, FAIL_SIGNAL_SIZE, serializedContent);
      }

      // Stop current task then go to the next one
      TaskRouter_nextTask();
    }

    // ...or there is still vacant address?
    else
    {
      // Parse received content
      unsigned int deviceClassSize = packet->contentSize + 1;
      char deviceClass[deviceClassSize];
      packet_content_RequestRegisterDevice_parse(packet->content, packet->contentSize, deviceClass);

      // Mark address assigned
      NetworkState_assign(NetworkState_nextVacantAddress, deviceClass, deviceClassSize);

      // Is send listener ready?
      if (m2tp_driver_sendListener != NULL)
      {
        // Create packet content for response
        packet_content_ResponseRegisterDevice responseContent;
        responseContent.address = NetworkState_nextVacantAddress;

        // Serialize that packet content
        m2tp_byte serializedResponseContent[RESPONSE_REGISTER_DEVICE_SIZE];
        packet_content_ResponseRegisterDevice_serialize(&responseContent, serializedResponseContent, NULL);

        // Send response
        m2tp_driver_sendListener(M2TP_COMMAND_RESPONSE_REGISTER_DEVICE, RESPONSE_REGISTER_DEVICE_SIZE, serializedResponseContent);

        // Introduce existing member to the new member
        packet_content_Reintroduction reintroductionContent;
        for (m2tp_byte address = 0; address < 128; address++)
        {
          // Skip if that address is not assigned
          if (!NetworkState_isAssigned(address))
            continue;

          reintroductionContent.address = address;
          reintroductionContent.deviceClass = (char *)NetworkState_deviceClasses[address];

          // Serialize the packet content
          m2tp_byte serializedContent[255];
          m2tp_byte contentSize = 0;
          packet_content_Reintroduction_serialize(
              &reintroductionContent,
              serializedContent,
              &contentSize);

          // Send a packet for introduction
          m2tp_driver_sendListener(M2TP_COMMAND_REINTRODUCTION, contentSize, serializedContent);
        }

        // Create packet content for new member announcement
        packet_content_AnnouncementJoin joinContent;
        joinContent.address = NetworkState_nextVacantAddress;
        joinContent.deviceClass = deviceClass;

        // Serialize that packet content
        m2tp_byte serializedJoinContent[255];
        m2tp_byte serializedJoinSize = 0;
        packet_content_AnnouncementJoin_serialize(&joinContent, serializedJoinContent, &serializedJoinSize);

        // Send new member announcement
        m2tp_driver_sendListener(M2TP_COMMAND_ANNOUNCEMENT_JOIN, serializedJoinSize, serializedJoinContent);

        // Notify to app about new member
        if (m2tp_onNewMemberListener != NULL)
          m2tp_onNewMemberListener(deviceClass, NetworkState_nextVacantAddress);
      }

      // Resolve the next vacant device address
      NetworkState_resolveNextVacantAddress();

      // Stop current task then go to the next one
      TaskRouter_nextTask();
    }
  }
  break;

  case M2TP_COMMAND_REQUEST_REGISTER_TOPIC:
  {
    // Parse received packet content
    char topicName[packet->contentSize + 1];
    packet_content_RequestRegisterTopic_parse(packet->content, packet->contentSize, topicName);

    // Check if topic name already registered
    m2tp_byte topicID = NetworkState_findTopic(topicName);

    // Is topic name registered?
    if (topicID != 0)
    {
      // Is send listener ready?
      if (m2tp_driver_sendListener)
      {
        // Create packet content for response
        packet_content_ResponseRegisterTopic responseContent;
        responseContent.ID = topicID;

        // Serialize that packet content
        m2tp_byte serializedContent[RESPONSE_REGISTER_TOPIC_SIZE];
        packet_content_ResponseRegisterTopic_serialize(&responseContent, serializedContent, NULL);

        // Send the response
        m2tp_driver_sendListener(M2TP_COMMAND_RESPONSE_REGISTER_TOPIC, RESPONSE_REGISTER_TOPIC_SIZE, serializedContent);
      }
    }

    // ...or topic name not registered yet?
    else
    {
      // Is topic full?
      if (NetworkState_isTopicFull)
      {
        // Is send listener ready?
        if (m2tp_driver_sendListener != NULL)
        {
          // Create packet content for fail signal
          packet_content_FailSignal failContent;
          failContent.errorCode = M2TP_ERROR_TOO_MUCH_TOPIC;

          // Serialize that packet content
          m2tp_byte serializedContent[FAIL_SIGNAL_SIZE];
          packet_content_FailSignal_serialize(&failContent, serializedContent, NULL);

          // Send fail signal
          m2tp_driver_sendListener(M2TP_COMMAND_FAIL_SIGNAL, FAIL_SIGNAL_SIZE, serializedContent);
        }
      }

      // ...or there is still unused topic ID?
      else
      {
        // Register the topic name
        NetworkState_assign(NetworkState_nextVacantTopicID, topicName, packet->contentSize + 1);

        // Is send listener ready?
        if (m2tp_driver_sendListener != NULL)
        {
          // Create packet content for response
          packet_content_ResponseRegisterTopic responseContent;
          responseContent.ID = NetworkState_nextVacantTopicID;

          // Serialize that packet content
          m2tp_byte serializedContent[RESPONSE_REGISTER_TOPIC_SIZE];
          packet_content_ResponseRegisterTopic_serialize(&responseContent, serializedContent, NULL);

          // Send the response
          m2tp_driver_sendListener(M2TP_COMMAND_RESPONSE_REGISTER_TOPIC, RESPONSE_REGISTER_TOPIC_SIZE, serializedContent);
        }

        // Resolve the next vacant topic ID
        NetworkState_resolveNextVacantTopicID();
      }
    }

    // Stop current task then go to the next one
    TaskRouter_nextTask();
  }
  break;

  default:
  {
    // Ask off-topic member to silence
    if (m2tp_driver_sendListener != NULL)
      m2tp_driver_sendListener(M2TP_COMMAND_END_TRANSMISSION, 0, NULL);

    // Restart timeout
    TaskRouter_startTimeout(RegistrationTask_TIMEOUT);
  }
  break;

  } // switch (packet->command)
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void RegistrationTask_start()
{
  // Assign functions to router
  TaskRouter_receiveInterrupt = &RegistrationTask_receiveInterrupt;
  TaskRouter_timeoutInterrupt = &TaskRouter_nextTask; // go to the next task on timeout

  // Send registration signal
  if (m2tp_driver_sendListener != NULL)
    m2tp_driver_sendListener(M2TP_COMMAND_REGISTRATION_SIGNAL, 0, NULL);

  // Start timer interrupt
  TaskRouter_startTimeout(RegistrationTask_TIMEOUT);
}

void RegistrationTask_stop()
{
  // Stop timeout
  TaskRouter_stopTimeout();

  // Detach functions
  TaskRouter_receiveInterrupt = NULL;
  TaskRouter_timeoutInterrupt = NULL;
}

////////////////////////////////////////////////////////
